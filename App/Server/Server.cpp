#include "Server.hpp"

_Noreturn void Server::ListenLoop()
{
	int max_fd = 0;
	int res;

	while (true)
	{
		Reset_fd_set();
		Add_new_fd_to_set();
		Search_max_fd(max_fd);
		res = select(max_fd + 1, &_readfds, &_writefds, nullptr, nullptr);
		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed();
		Check_read_set();
		Check_write_set();
	}
}

void Server::Act_if_writefd_changed(std::list<Client>::iterator &Iter)
{
	int counter;
	if (Iter->_ready_response_to_the_customer.size() > 32768)
	{
		if ((counter = send(Iter->_client_fd, Iter->_ready_response_to_the_customer.substr(0, 32768).c_str(), 32768, MSG_DONTWAIT)) < 0)
		{
			++Iter;
			return;
		}
		std::cout << counter << std::endl;
		Iter->_ready_response_to_the_customer.erase(0, counter);
		++Iter;
		return;
	}
	if (send(Iter->_client_fd, Iter->_ready_response_to_the_customer.c_str(), Iter->_ready_response_to_the_customer.size(), 0) < 0)
	{
		++Iter;
		return;
	}
	if (Iter->_client_handler->getVariableHandlers().find("Connection") != Iter->_client_handler->getVariableHandlers().end() &&
		Iter->_client_handler->getVariableHandlers().at("Connection") == "close")
	{
		close(Iter->_client_fd);
		_clients.erase(Iter);
	}
	else
	{
		Iter->_answer_is_ready = false;
		Iter->_curent_progress = 0;
	}
}

void Server::Act_if_readfd_changed(std::list<Client>::iterator &Iter)
{
	std::string handler;
	std::string body;

	if (Iter->_curent_progress == 0 && Reading_a_request(Iter))
		return;
	if (Iter->_curent_progress == 1 &&
		(Iter->_client_handler->getVariableHandlers().find("Content-Length") != Iter->_client_handler->getVariableHandlers().end()))
		if (read_with_content_length(std::stoi(Iter->_client_handler->getVariableHandlers().at("Content-Length")), Iter))
		{
			++Iter;
			return;
		}
	if (Iter->_curent_progress == 1 &&
		(Iter->_client_handler->getVariableHandlers().find("Transfer-Encoding") != Iter->_client_handler->getVariableHandlers().end()))
		if (!read_with_chunked(Iter))
		{
			++Iter;
			return;
		}
	Method_selector(*Iter->_client_handler, handler, body, Iter->_request_body);
	Iter->_ready_response_to_the_customer = handler + body;
	Iter->_answer_is_ready = true;
	++Iter;
}

bool Server::read_with_content_length(int size, std::list<Client>::iterator &Iter)
{
	int count;
	char *buff;

	if (!Iter->_content_length_buffer)
		Iter->_content_length_buffer = size;
	buff = new char[Iter->_content_length_buffer + 1];
	count = recv(Iter->_client_fd, buff, Iter->_content_length_buffer, 0);
	if (count == -1)
		return true;
	buff[count] = '\0';
	Iter->_request_body += buff;
	if (count < Iter->_content_length_buffer)
	{
		Iter->_content_length_buffer -= count;
		return true;
	}
	delete[] buff;
	return false;
}

bool Server::read_with_chunked(std::list<Client>::iterator &Iter)
{
	int tmp_chunked_length;
	int count = 0;
	char *buff;
	char crlf_buffer[5];
	std::string checker;
	std::stringstream tmp;

	if (Iter->_chunked_length_status)
	{
		if (!Iter->_chunked_length)
		{
			count = recv(Iter->_client_fd, crlf_buffer, 2, MSG_PEEK);
			if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2) != 0)
				return false;
			if (!std::strncmp(crlf_buffer, "\r\n", 2))
			{
				recv(Iter->_client_fd, crlf_buffer, 2, 0);
				Iter->_chunked_length_status = false;
			}
			return false;
		}
		buff = new char[Iter->_chunked_length + 1];
		count = recv(Iter->_client_fd, buff, Iter->_chunked_length, 0); // Считываем колличесво байт, которое было указано в первом блоке
		std::cout << "Line_length: " << count << std::endl;
		if (count == -1)
			return false;
		buff[count] = '\0';
		tmp_chunked_length = Iter->_chunked_length;
		Iter->_chunked_length -= count;
		std::cout << GREEN << "Chunked_len: " << Iter->_chunked_length << std::endl << RESET;
		Iter->_request_body += buff;
		if (count < tmp_chunked_length)
		{
			delete[] buff;
			return false;
		}
		count = recv(Iter->_client_fd, crlf_buffer, 2, MSG_PEEK);
		if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2) != 0)
			return false;
		if (!std::strncmp(crlf_buffer, "\r\n", 2))
		{
			recv(Iter->_client_fd, crlf_buffer, 2, 0);
			Iter->_chunked_length_status = false;
		}
		return false;
	}
	if (!Iter->_chunked_end_check_status)
	{
		buff = new char[256];
		count = recv(Iter->_client_fd, buff, 255, MSG_PEEK); // выделяем память для просмотра колличества букв в chunked
		if (count == -1) // если читать нечего просто выходим для нового прохода через select
			return false;
		buff[count] = '\0';
		checker = buff;
		count = checker.find("\r\n"); //находит CRLF последовательность
		if (count == std::string::npos)
			return false;
		count = recv(Iter->_client_fd, buff, count, 0); // Читаем до CRLF последовательности
		if (count == -1) // если пришло только число
			return false;
		buff[count] = '\0';
		checker = buff;
		tmp << checker; // Переводим из HEX в DEC
		tmp >> std::hex >> count;
		std::cout << GREEN << "Chunked_len: " << count << std::endl << RESET;
	}
	if (!count || Iter->_chunked_end_check_status)
	{
		Iter->_chunked_end_check_status = true;
		count = recv(Iter->_client_fd, crlf_buffer, 4, 0);
		if (count == -1)
			return false;
		crlf_buffer[count] = '\0';
		Iter->_chunked_end_check += crlf_buffer;
		if (!std::strncmp(crlf_buffer, "\r\n\r\n", 4) || !std::strncmp(Iter->_chunked_end_check.c_str(), "\r\n\r\n", 4))
		{
			Iter->_chunked_end_check_status = false;
			return true;
		}
		return false;
	}
	else
	{
		Iter->_chunked_length_status = true;
		Iter->_chunked_length = count;
	}
	recv(Iter->_client_fd, crlf_buffer, 2, 0); // можно не проверять на возможность прочитать, так как мы сюда не попадем до момента, пока не не сможем прочитать число + CRLF
	delete[] buff;
	buff = new char[Iter->_chunked_length + 1];
	count = recv(Iter->_client_fd, buff, Iter->_chunked_length, 0); // Считываем колличесво байт, которое было указано в первом блоке
	std::cout << "Line_length: " << count << std::endl;
	if (count == -1)
		return false;
	buff[count] = '\0';
	tmp_chunked_length = Iter->_chunked_length;
	Iter->_chunked_length -= count;
	std::cout << GREEN << "Chunked_len: " << Iter->_chunked_length << std::endl << RESET;
	Iter->_request_body += buff;
	if (count < tmp_chunked_length)
	{
		delete[] buff;
		return false;
	}
	count = recv(Iter->_client_fd, crlf_buffer, 2, MSG_PEEK);
	if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2) != 0)
		return false;
	if (!std::strncmp(crlf_buffer, "\r\n", 2))
	{
		recv(Iter->_client_fd, crlf_buffer, 2, 0);
		Iter->_chunked_length_status = false;
	}
	delete[] buff;
	return false;
}

bool Server::Reading_a_request(std::list<Client>::iterator &Iter)
{
	char *buffer_for_request;
	char *output;
	int request_size;

	buffer_for_request = new char[576];
	bzero(buffer_for_request, 576);
	request_size = recv(Iter->_client_fd, buffer_for_request, 575, MSG_PEEK);
	if (request_size > 0)
		buffer_for_request[request_size] = '\0';
	if (request_size == 0)
	{
		close(Iter->_client_fd);
		_clients.erase(Iter);
		delete[] buffer_for_request;
		return true;
	}
	if (request_size == -1 || (output = check_input_handler_buffer(buffer_for_request, Iter)) == nullptr)
	{
		++Iter;
		delete[] buffer_for_request;
		return true;
	}
	std::cout << output << std::endl;
	Iter->_client_handler = new Parse_input_handler(output, Iter->_server_client_ip);
	++Iter->_curent_progress;
	delete[] buffer_for_request;
	return false;
}

char *Server::check_input_handler_buffer(char *input_buffer, std::list<Client>::iterator &Iter)
{
	int pos;
	char *tmp_return_buffer;
	std::string check_buffer;

	if (!Iter->_request_header.empty())
	{

		check_buffer = Iter->_request_header + input_buffer;
		if ((pos = check_buffer.find("\r\n\r\n")) != std::string::npos)
		{
			pos -= Iter->_request_header.size();
			pos = recv(Iter->_client_fd, input_buffer, pos + 4, 0);
			if (pos == -1)
				return nullptr;
			if (pos > 0)
				input_buffer[pos] = '\0';
			tmp_return_buffer = strdup((Iter->_request_header + input_buffer).c_str());
			Iter->_request_header = "";
			return tmp_return_buffer;
		}
	}
	check_buffer = input_buffer;
	if ((pos = check_buffer.find("\r\n\r\n")) != std::string::npos)
	{
		pos = recv(Iter->_client_fd, input_buffer, pos + 4, 0);
		if (pos == -1)
			return nullptr;
		if (pos > 0)
			input_buffer[pos] = '\0';
		if (Iter->_request_header.empty())
			return strdup(input_buffer);
		tmp_return_buffer = strdup((Iter->_request_header + input_buffer).c_str());
		Iter->_request_header = "";
		return tmp_return_buffer;
	}
	else
	{
		bzero(input_buffer, 575);
		pos = recv(Iter->_client_fd, input_buffer, check_buffer.size(), 0);
		if (pos == -1)
			return nullptr;
		if (pos > 0)
			input_buffer[pos] = '\0';
		Iter->_request_header += input_buffer;
	}
	return nullptr;
}
