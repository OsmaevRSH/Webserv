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
	//	int send_number;
	//	send_number =
	send(*Iter, (_request_to_client[*Iter][0] + _request_to_client[*Iter][1]).c_str(),
			_request_to_client[*Iter][0].size() + _request_to_client[*Iter][1].size(), 0);
	//	if (send_number < static_cast<int>(_request_to_client[*Iter][0].size() +
	//									   _request_to_client[*Iter][1].size()))
	//	{
	//		if (send_number <=
	//			static_cast<int>(_request_to_client[*Iter][0].size()))
	//			_request_to_client[*Iter][0].erase(0, send_number);
	//		else
	//		{
	//			_request_to_client[*Iter][1].erase(0,
	//					send_number - _request_to_client[*Iter][0].size());
	//			_request_to_client[*Iter][0].erase(_request_to_client[*Iter][0].begin(), _request_to_client[*Iter][0].end());
	//		}
	//	}
	//	else
	//	{
	//		close(*Iter);
	_request_to_client.erase(*Iter);
	_read_socket_fd.push_back(*Iter);
	Iter = _write_socket_fd.erase(Iter);
	//	}
}

void Server::Act_if_readfd_changed(std::vector<int>::iterator &Iter)
{
	std::vector<std::string> tmp;
	Parse_input_handler *inputHandlers;
	std::string handler;
	std::string body;

	if (_body_in_request.find(*Iter) == _body_in_request.end() && (inputHandlers = Reading_a_request(Iter)) == nullptr)
		return;
	if ((inputHandlers->getVariableHandlers().find("Content-length") != inputHandlers->getVariableHandlers().end()))
		read_with_content_length(std::stoi(inputHandlers->getVariableHandlers().at("Content-length")), Iter);
	if ((inputHandlers->getVariableHandlers().find("Transfer-Encoding") != inputHandlers->getVariableHandlers().end()))
		if (!read_with_chunked(*Iter))
			return;
#ifdef SERVER_DEBUG
	inputHandlers.output();
#endif
	Method_selector(*inputHandlers, handler, body);
	tmp.push_back(handler);
	tmp.push_back(body);
	_request_to_client.insert(std::pair<int, std::vector<std::string> >(*Iter, tmp));
	_write_socket_fd.push_back(*Iter);
	Iter = _read_socket_fd.erase(Iter);
}

void Server::read_with_content_length(int size, std::vector<int>::iterator &Iter)
{
	int count;
	char *buff;

	buff = new char[size + 1];
	count = recv(*Iter, buff, size, 0);
	buff[count] = '\0';
	_body_in_request.insert(std::pair<int, std::string>(*Iter, buff));
	delete[] buff;
}

bool Server::read_with_chunked(int fd)
{
	int count;
	char *buff;
	std::string checker;
	std::stringstream tmp;

	buff = new char[256];
	count = recv(fd, buff, 255, MSG_PEEK); // выделяем память для просмотра колличества букв в chunked
	buff[count] = '\0';
	checker = buff;
	count = checker.find("\r\n"); //находит CRLF последовательность
	count = recv(fd, buff, count, 0); // Читаем до CRLF последовательности
	buff[count] = '\0';
	checker = buff;
	tmp << std::hex << checker; // Переводим из HEX в DEC
	recv(fd, nullptr, 2, 0);
	count = std::stoi(tmp.str());
	if (!count)
	{
		recv(fd, nullptr, 4, 0);
		return true;
	}
	count = recv(fd, buff, count, 0); // Считываем колличесво байт, которое было указано в первом блоке
	buff[count] = '\0';
	recv(fd, nullptr, 2, 0);
	checker = buff;
	if (_body_in_request.find(fd) == _body_in_request.end())
		_body_in_request.insert(std::pair<int, std::string>(fd, checker));
	else
		_body_in_request[fd] += checker;
	return false;
}

Parse_input_handler *Server::Reading_a_request(std::vector<int>::iterator &Iter)
{
	char *buffer_for_request;
	char *output;
	int request_size;
	Parse_input_handler *inputHandlers;

	buffer_for_request = new char[576];
	request_size = recv(*Iter, buffer_for_request, 575, MSG_PEEK);
	if (request_size > 0)
		buffer_for_request[request_size] = '\0';
	if (request_size == 0 && _request_to_client.find(*Iter) == _request_to_client.end())
	{
		close(*Iter);
		Iter = _read_socket_fd.erase(Iter);
		delete[] buffer_for_request;
		return nullptr;
	}
	if (request_size == -1 || (output = check_input_handler_buffer(buffer_for_request, Iter)) == nullptr)
	{
		++Iter;
		delete[] buffer_for_request;
		return nullptr;
	}
	inputHandlers = new Parse_input_handler(output);
	if ((inputHandlers->getVariableHandlers().find("Connection") != inputHandlers->getVariableHandlers().end() &&
		 inputHandlers->getVariableHandlers().at("Connection") == "close"))
	{
		close(*Iter);
		Iter = _read_socket_fd.erase(Iter);
		delete[] buffer_for_request;
		return nullptr;
	}
	delete[] buffer_for_request;
	return inputHandlers;
}

char *Server::check_input_handler_buffer(char *input_buffer, std::vector<int>::iterator &Iter)
{
	int pos;
	char *tmp_return_buffer;
	std::string check_buffer;

	if (_input_handler_buffer.find(*Iter) != _input_handler_buffer.end())
	{
		check_buffer = _input_handler_buffer[*Iter] + input_buffer;
		if ((pos = check_buffer.find("\r\n\r\n")) != std::string::npos)
		{
			pos -= _input_handler_buffer[*Iter].size();
			pos = recv(*Iter, input_buffer, pos + 4, 0);
			if (pos > 0)
				input_buffer[pos] = '\0';
			tmp_return_buffer = strdup((_input_handler_buffer[*Iter] + input_buffer).c_str());
			_input_handler_buffer.erase(*Iter);
			return tmp_return_buffer;
		}
	}
	check_buffer = input_buffer;
	if ((pos = check_buffer.find("\r\n\r\n")) != std::string::npos)
	{
		pos = recv(*Iter, input_buffer, pos + 4, 0);
		if (pos > 0)
			input_buffer[pos] = '\0';
		if (_input_handler_buffer.find(*Iter) == _input_handler_buffer.end())
			return strdup(input_buffer);
		tmp_return_buffer = strdup((_input_handler_buffer[*Iter] + input_buffer).c_str());
		_input_handler_buffer.erase(*Iter);
		return tmp_return_buffer;
	}
	else
	{
		pos = recv(*Iter, input_buffer, 575, 0);
		if (pos > 0)
			input_buffer[pos] = '\0';
		if (_input_handler_buffer.find(*Iter) == _input_handler_buffer.end())
			_input_handler_buffer.insert(std::pair<int, std::string>(*Iter, input_buffer));
		else
			_input_handler_buffer[*Iter] += input_buffer;
	}
	return nullptr;
}
