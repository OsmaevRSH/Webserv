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

void Server::Act_if_writefd_changed(std::vector<int>::iterator &Iter)
{
	int counter = 0;
	if (_ready_response_to_the_customer[*Iter].size() > 32768)
	{
		if ((counter = send(*Iter, (_ready_response_to_the_customer[*Iter]).substr(0, 32768).c_str(), 32768, MSG_DONTWAIT)) < 0)
		{
			++Iter;
			return;
		}
		std::cout << counter << std::endl;
		_ready_response_to_the_customer[*Iter].erase(0, counter);
		++Iter;
		return;
	}
	if (send(*Iter, (_ready_response_to_the_customer[*Iter]).c_str(), _ready_response_to_the_customer[*Iter].size(), 0) < 0)
	{
		++Iter;
		return;
	}
	if (_edited_headers[*Iter]->getVariableHandlers().find("Connection") != _edited_headers[*Iter]->getVariableHandlers().end() &&
		_edited_headers[*Iter]->getVariableHandlers().at("Connection") == "close")
	{
		close(*Iter);
		_ready_response_to_the_customer.erase(*Iter);
		_edited_headers.erase(*Iter);
		_read_socket_fd.erase(Iter);
		Iter = _write_socket_fd.erase(--Iter);
	}
	else
	{
		_ready_response_to_the_customer.erase(*Iter);
		_read_socket_fd.push_back(*Iter);
		Iter = _write_socket_fd.erase(Iter);
	}
}

void Server::Act_if_readfd_changed(std::vector<int>::iterator &Iter)
{
	std::string handler;
	std::string body;

	if (_request_body.find(*Iter) == _request_body.end() && _edited_headers.find(*Iter) == _edited_headers.end() && Reading_a_request(Iter))
		return;
	if ((_edited_headers.find(*Iter) != _edited_headers.end()) &&
		(_edited_headers[*Iter]->getVariableHandlers().find("Content-Length") != _edited_headers[*Iter]->getVariableHandlers().end()))
		if (read_with_content_length(std::stoi(_edited_headers[*Iter]->getVariableHandlers().at("Content-Length")), *Iter))
		{
			++Iter;
			return;
		}
	if ((_edited_headers.find(*Iter) != _edited_headers.end()) &&
		(_edited_headers[*Iter]->getVariableHandlers().find("Transfer-Encoding") != _edited_headers[*Iter]->getVariableHandlers().end()))
		if (!read_with_chunked(*Iter))
		{
			++Iter;
			return;
		}
	Method_selector(*_edited_headers[*Iter], handler, body, _request_body[*Iter]);
	_request_body.erase(*Iter);
	_ready_response_to_the_customer.insert(std::pair<int, std::string>(*Iter, handler + body));
	_write_socket_fd.push_back(*Iter);
	++Iter;
}

bool Server::read_with_content_length(int size, int fd)
{
	std::map<int, int> &_content_length_buffer = _chunked_length;
	int count;
	char *buff;

	if (_content_length_buffer.find(fd) == _content_length_buffer.end())
		_content_length_buffer.insert(std::pair<int, int>(fd, size));
	if (_request_body.find(fd) == _request_body.end())
		_request_body.insert(std::pair<int, std::string>(fd, ""));
	buff = new char[_content_length_buffer[fd] + 1];
	count = recv(fd, buff, _content_length_buffer[fd], 0);
	if (count == -1)
		return true;
	buff[count] = '\0';
	_request_body[fd] += buff;
	if (count < _content_length_buffer[fd])
	{
		_content_length_buffer[fd] -= count;
		return true;
	}
	delete[] buff;
	return false;
}

bool Server::read_with_chunked(int fd)
{
	int tmp_chunked_length;
	int count = 0;
	char *buff;
	char crlf_buffer[5];
	std::string checker;
	std::stringstream tmp;

	if (_chunked_length.find(fd) != _chunked_length.end())
	{
		if (!_chunked_length[fd])
		{
			count = recv(fd, crlf_buffer, 2, MSG_PEEK);
			if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2))
				return false;
			if (!std::strncmp(crlf_buffer, "\r\n", 2))
			{
				recv(fd, crlf_buffer, 2, 0);
				_chunked_length.erase(fd);
			}
			return false;
		}
		buff = new char[_chunked_length[fd] + 1];
		count = recv(fd, buff, _chunked_length[fd], 0); // Считываем колличесво байт, которое было указано в первом блоке
		std::cout << "Line_length: " << count << std::endl;
		if (count == -1)
			return false;
		buff[count] = '\0';
		tmp_chunked_length = _chunked_length[fd];
		_chunked_length[fd] -= count;
		std::cout << GREEN << "Chunked_len: " << _chunked_length[fd] << std::endl << RESET;
		_request_body[fd] += buff;
		if (count < tmp_chunked_length)
		{
			delete[] buff;
			return false;
		}
		count = recv(fd, crlf_buffer, 2, MSG_PEEK);
		if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2))
			return false;
		if (!std::strncmp(crlf_buffer, "\r\n", 2))
		{
			recv(fd, crlf_buffer, 2, 0);
			_chunked_length.erase(fd);
		}
		return false;
	}
	if (_chunked_end_check.find(fd) == _chunked_end_check.end())
	{
		buff = new char[256];
		count = recv(fd, buff, 255, MSG_PEEK); // выделяем память для просмотра колличества букв в chunked
		if (count == -1) // если читать нечего просто выходим для нового прохода через select
			return false;
		buff[count] = '\0';
		checker = buff;
		count = checker.find("\r\n"); //находит CRLF последовательность
		if (count == std::string::npos)
			return false;
		count = recv(fd, buff, count, 0); // Читаем до CRLF последовательности
		if (count == -1) // если пришло только число
			return false;
		buff[count] = '\0';
		checker = buff;
		tmp << checker; // Переводим из HEX в DEC
		tmp >> std::hex >> count;
		std::cout << GREEN << "Chunked_len: " << count << std::endl << RESET;
	}
	if (!count || _chunked_end_check.find(fd) != _chunked_end_check.end())
	{
		if (_chunked_end_check.find(fd) == _chunked_end_check.end())
			_chunked_end_check.insert(std::pair<int, std::string>(fd, ""));
		count = recv(fd, crlf_buffer, 4, 0);
		if (count == -1)
			return false;
		crlf_buffer[count] = '\0';
		_chunked_end_check[fd] += crlf_buffer;
		if (!std::strncmp(crlf_buffer, "\r\n\r\n", 4) || !std::strncmp(_chunked_end_check[fd].c_str(), "\r\n\r\n", 4))
		{
			_chunked_end_check.erase(fd);
			return true;
		}
		return false;
	}
	else
	{
		if (_chunked_length.find(fd) == _chunked_length.end())
			_chunked_length.insert(std::pair<int, int>(fd, count));
		if (_request_body.find(fd) == _request_body.end())
			_request_body.insert(std::pair<int, std::string>(fd, ""));
	}
	recv(fd, crlf_buffer, 2, 0); // можно не проверять на возможность прочитать, так как мы сюда не попадем до момента, пока не не сможем прочитать число + CRLF
	delete[] buff;
	buff = new char[_chunked_length[fd] + 1];
	count = recv(fd, buff, _chunked_length[fd], 0); // Считываем колличесво байт, которое было указано в первом блоке
	std::cout << "Line_length: " << count << std::endl;
	if (count == -1)
		return false;
	buff[count] = '\0';
	tmp_chunked_length = _chunked_length[fd];
	_chunked_length[fd] -= count;
	std::cout << GREEN << "Chunked_len: " << _chunked_length[fd] << std::endl << RESET;
	_request_body[fd] += buff;
	if (count < tmp_chunked_length)
	{
		delete[] buff;
		return false;
	}
	count = recv(fd, crlf_buffer, 2, MSG_PEEK);
	if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2))
		return false;
	if (!std::strncmp(crlf_buffer, "\r\n", 2))
	{
		recv(fd, crlf_buffer, 2, 0);
		_chunked_length.erase(fd);
	}
	delete[] buff;
	return false;
}

bool Server::Reading_a_request(std::vector<int>::iterator &Iter)
{
	char *buffer_for_request;
	char *output;
	int request_size;

	buffer_for_request = new char[576];
	bzero(buffer_for_request, 576);
	request_size = recv(*Iter, buffer_for_request, 575, MSG_PEEK);
	if (request_size > 0)
		buffer_for_request[request_size] = '\0';
	if (request_size == 0)
	{
		close(*Iter);
		_edited_headers.erase(*Iter);
		_server_client_ip.erase(*Iter);
		_ready_response_to_the_customer.erase(*Iter);
		_write_socket_fd.erase(Iter);
		_read_socket_fd.erase(--Iter);
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
	if (_edited_headers.find(*Iter) == _edited_headers.end())
		_edited_headers.insert(std::pair<int, Parse_input_handler *>(*Iter, nullptr));
	_edited_headers[*Iter] = new Parse_input_handler(output, _server_client_ip[*Iter]);
	delete[] buffer_for_request;
	return false;
}

char *Server::check_input_handler_buffer(char *input_buffer, std::vector<int>::iterator &Iter)
{
	int pos;
	char *tmp_return_buffer;
	std::string check_buffer;

	if (_request_header.find(*Iter) != _request_header.end())
	{

		check_buffer = _request_header[*Iter] + input_buffer;
		if ((pos = check_buffer.find("\r\n\r\n")) != std::string::npos)
		{
			pos -= _request_header[*Iter].size();
			pos = recv(*Iter, input_buffer, pos + 4, 0);
			if (pos == -1)
				return nullptr;
			if (pos > 0)
				input_buffer[pos] = '\0';
			tmp_return_buffer = strdup((_request_header[*Iter] + input_buffer).c_str());
			_request_header.erase(*Iter);
			return tmp_return_buffer;
		}
	}
	check_buffer = input_buffer;
	if ((pos = check_buffer.find("\r\n\r\n")) != std::string::npos)
	{
		pos = recv(*Iter, input_buffer, pos + 4, 0);
		if (pos == -1)
			return nullptr;
		if (pos > 0)
			input_buffer[pos] = '\0';
		if (_request_header.find(*Iter) == _request_header.end())
			return strdup(input_buffer);
		tmp_return_buffer = strdup((_request_header[*Iter] + input_buffer).c_str());
		_request_header.erase(*Iter);
		return tmp_return_buffer;
	}
	else
	{
		bzero(input_buffer, 575);
		pos = recv(*Iter, input_buffer, check_buffer.size(), 0);
		if (pos == -1)
			return nullptr;
		if (pos > 0)
			input_buffer[pos] = '\0';
		if (_request_header.find(*Iter) == _request_header.end())
			_request_header.insert(std::pair<int, std::string>(*Iter, input_buffer));
		else
			_request_header[*Iter] += input_buffer;
	}
	return nullptr;
}
