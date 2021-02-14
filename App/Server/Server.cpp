#include "Server.hpp"

Server::Server(const serv_vec &serv, const errp_map &err, const ew_str &ew, MIME_ERROR &mime, char **env, int family, int type, int protocol)
		: _config(serv, err, ew), _type(type), _family(family), _protocol(protocol), _readfds(), _writefds(), _mime(mime), env(env) {}

Server::~Server() { unlink(".tmp_cgi"); }

void Server::Socket()
{
	_master_socket_fd.reserve(_config._servers.size());
	for (int i = 0; i < static_cast<int>(_config._servers.size()); ++i)
	{
		_master_socket_fd.push_back(socket(_family, _type, _protocol));
		if (*_master_socket_fd.rbegin() == -1)
		{
			perror("Create socket error");
			exit(EXIT_FAILURE);
		}
		Set_non_blocked(*_master_socket_fd.rbegin());
	}
}

void Server::Bind()
{
	std::vector<ConfigParser::s_server>::iterator Iter;
	std::vector<int>::iterator Iter_fd;
	Iter = _config._servers.begin();
	Iter_fd = _master_socket_fd.begin();
#ifdef SERVER_IP_DEBUG
	int i = 0;
#endif
	int bind_res;
	struct sockaddr_in addr = {};

	for (; Iter < _config._servers.end(); ++Iter, ++Iter_fd)
	{
		bzero(&addr, sizeof(addr));
		addr.sin_family = _family;
		if (Iter->port == 0)
			Iter->port = 8080;
		if (Iter->ip.empty())
			Iter->ip = "0.0.0.0";
		addr.sin_port = htons(Iter->port);
		addr.sin_addr.s_addr = inet_addr(Iter->ip.c_str());

		int opt = 1;
		setsockopt(*Iter_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		bind_res = bind(*Iter_fd, (struct sockaddr *) (&addr), sizeof(addr));
		if (bind_res == -1)
		{
			perror("Create bind error");
			exit(EXIT_FAILURE);
		}
#ifdef SERVER_IP_DEBUG
		std::cout << "Server[" << i++ + 1 << "]:[" << Iter->ip << ":" << Iter->port << "]" << std::endl;
#endif
	}
}

void Server::Listen() const
{
	std::vector<int>::const_iterator it;
	it = _master_socket_fd.begin();

	for (; it < _master_socket_fd.end(); ++it)
	{
		listen(*it, SOMAXCONN);
	}
}

void Server::Accept(int fd)
{
	static int client_num = 0;
	Client client;
	char str[32];
	struct sockaddr_in addr = {0};
	struct sockaddr_in client_addr = {0};
	socklen_t addr_len;
	socklen_t client_addr_len;
	int new_client_fd;
	addr_len = sizeof(addr);
	client_addr_len = sizeof(client_addr);
	new_client_fd = accept(fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	getsockname(new_client_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_len);
	std::cout << "Client №" << client_num << " Client fd: " << new_client_fd << std::endl;
	++client_num;
	if (new_client_fd == -1)
		return;
	inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
	client._server_ip = str;
	inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
	client._client_ip = str;
	client._client_fd = new_client_fd;
	_clients.push_back(client);
	Set_non_blocked(new_client_fd);
}

void Server::server_start()
{
	this->Socket();
	this->Bind();
	this->Listen();
	this->ListenLoop();
}

void Server::Set_non_blocked(int fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

void Server::Reset_fd_set()
{
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
}

void Server::Add_new_fd_to_set()
{
	std::vector<int>::iterator Iter_1;
	std::list<Client>::iterator Iter_2;

	for (Iter_1 = _master_socket_fd.begin(); Iter_1 < _master_socket_fd.end(); ++Iter_1)
		FD_SET(*Iter_1, &_readfds);
	for (Iter_2 = _clients.begin(); Iter_2 != _clients.end(); ++Iter_2)
		if (!Iter_2->_answer_is_ready)
			FD_SET(Iter_2->_client_fd, &_readfds);
	for (Iter_2 = _clients.begin(); Iter_2 != _clients.end(); ++Iter_2)
		if (Iter_2->_answer_is_ready)
			FD_SET(Iter_2->_client_fd, &_writefds);
}

void Server::Accept_if_serv_fd_changed()
{
	std::vector<int>::iterator it;
	it = _master_socket_fd.begin();
	for (; it < _master_socket_fd.end(); ++it)
		if (FD_ISSET(*it, &_readfds))
			Accept(*it);
}

void Server::Search_max_fd(int &max_fd)
{
	std::list<Client>::iterator Iter;
	std::vector<int>::iterator Iter_1;

	int read_max = 0;
	int write_max = 0;
	int master_max = 0;

	if (!_clients.empty())
	{
		for (Iter = _clients.begin(); Iter != _clients.end(); ++Iter)
			if (!Iter->_answer_is_ready)
				read_max = Iter->_client_fd > read_max ? Iter->_client_fd : read_max;
		for (Iter = _clients.begin(); Iter != _clients.end(); ++Iter)
			if (Iter->_answer_is_ready)
				write_max = Iter->_client_fd > write_max && Iter->_answer_is_ready ? Iter->_client_fd : write_max;
	}
	if (!_master_socket_fd.empty())
		for (Iter_1 = _master_socket_fd.begin(); Iter_1 != _master_socket_fd.end(); ++Iter_1)
			master_max = *Iter_1 > master_max ? *Iter_1 : master_max;
	max_fd = std::max(read_max, write_max);
	max_fd = std::max(max_fd, master_max);
}

//void Server::Checkout_call_to_select(const int &res)
//{
//	if (res <= 0)
//	{
//		if (errno == EINTR)
//		{
//			perror("Select error");
//			exit(1);
//		}
//		else if (errno == EBADF)
//		{
//			perror("Select error");
//			exit(2);
//		}
//		else if (errno == EINVAL)
//		{
//			perror("Select error");
//			exit(3);
//		}
//		else if (errno == ENOMEM)
//		{
//			perror("Select error");
//			exit(4);
//		}
//		else
//			exit(0);
//	}
//TODO Delete client
//}

void Server::Check_read_set()
{
	std::list<Client>::iterator Iter;

	Iter = _clients.begin();
	while (Iter != _clients.end())
	{
		if (!Iter->_answer_is_ready && FD_ISSET(Iter->_client_fd, &_readfds))
			Act_if_readfd_changed(Iter);
		else
			++Iter;
	}
}

void Server::Check_write_set()
{
	std::list<Client>::iterator Iter;

	Iter = _clients.begin();
	while (Iter != _clients.end())
	{
		if (Iter->_answer_is_ready && FD_ISSET(Iter->_client_fd, &_writefds))
			Act_if_writefd_changed(Iter);
		else
			++Iter;
	}
}

void Server::ListenLoop()
{
	int max_fd = 0;
	int res;

	while (true)
	{
		Reset_fd_set();
		Add_new_fd_to_set();
		Search_max_fd(max_fd);
		/*res = */select(max_fd + 1, &_readfds, &_writefds, nullptr, nullptr);
//		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed();
		Check_read_set();
		Check_write_set();
	}
}

void Server::Act_if_writefd_changed(std::list<Client>::iterator &Iter)
{
	int counter;
	int tmp_count;
	if (Iter->_ready_response_to_the_customer.size() > 1000000)
	{
		if ((counter = send(Iter->_client_fd, Iter->_ready_response_to_the_customer.substr(0, 1000000).c_str(), 1000000, MSG_DONTWAIT)) < 0)
		{
			++Iter;
			return;
		}
		Iter->_ready_response_to_the_customer.erase(0, counter);
		++Iter;
		return;
	}
	tmp_count = Iter->_ready_response_to_the_customer.size();
	if ((counter = send(Iter->_client_fd, Iter->_ready_response_to_the_customer.c_str(), Iter->_ready_response_to_the_customer.size(), 0)) < 0)
	{
		++Iter;
		return;
	}
	if (counter != tmp_count)
	{
		Iter->_ready_response_to_the_customer.erase(0, counter);
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
		Iter->resetClient();
		++Iter;
	}
}

void Server::Act_if_readfd_changed(std::list<Client>::iterator &Iter)
{
	std::string handler;
	std::string body;

	if (Iter->_curent_progress == 0 && Reading_a_request(Iter))
		return;
	if (Iter->_curent_progress == 1 && !Iter->_client_handler->isError() &&
		Iter->_client_handler->getVariableHandlers().find("Content-Length") != Iter->_client_handler->getVariableHandlers().end())
		if (read_with_content_length(std::stoi(Iter->_client_handler->getVariableHandlers().at("Content-Length")), Iter))
		{
			++Iter;
			return;
		}
	if (Iter->_curent_progress == 1 && !Iter->_client_handler->isError() &&
		(Iter->_client_handler->getVariableHandlers().find("Transfer-Encoding") != Iter->_client_handler->getVariableHandlers().end()))
		if (!read_with_chunked(Iter))
		{
			++Iter;
			return;
		}
	if (!Iter->_client_handler->isError())
		Method_selector(handler, body, Iter);
	else
	{
		body = _mime.get_error_page(400);
		handler = "HTTP/1.1 400 BAD REQUEST\r\n"
				  "Content-Type: text/html\r\n"
	              "Content-Length: " + std::to_string(body.size()) + "\r\n"
				  "Server: Webserver/1.0\r\n\r\n";
	}
	std::cout << RED << handler << RESET << std::endl;
	Iter->answerDone(body, handler);
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
	{
		delete[] buff;
		return true;
	}
	buff[count] = '\0';
	Iter->_request_body.append(buff);
	if (count < Iter->_content_length_buffer)
	{
		Iter->_content_length_buffer -= count;
		delete[] buff;
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
				Iter->_chunked_length = 0;
			}
			return false;
		}
		buff = new char[Iter->_chunked_length + 1];
		count = recv(Iter->_client_fd, buff, Iter->_chunked_length, 0); // Считываем колличесво байт, которое было указано в первом блоке
		if (count == -1)
		{
			delete[] buff;
			return false;
		}
		buff[count] = '\0';
		tmp_chunked_length = Iter->_chunked_length;
		Iter->_chunked_length -= count;
		Iter->_request_body.append(buff);
		delete[] buff;
		if (count < tmp_chunked_length)
			return false;
		count = recv(Iter->_client_fd, crlf_buffer, 2, MSG_PEEK);
		if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2) != 0)
			return false;
		if (!std::strncmp(crlf_buffer, "\r\n", 2))
		{
			recv(Iter->_client_fd, crlf_buffer, 2, 0);
			Iter->_chunked_length_status = false;
			Iter->_chunked_length = 0;
		}
		return false;
	}
	if (!Iter->_chunked_end_check_status)
	{
		buff = new char[256];
		count = recv(Iter->_client_fd, buff, 255, MSG_PEEK); // выделяем память для просмотра колличества букв в chunked
		if (count == -1) // если читать нечего просто выходим для нового прохода через select
		{
			delete[] buff;
			return false;
		}
		buff[count] = '\0';
		checker = buff;
		count = checker.find("\r\n"); //находит CRLF последовательность
		if (count == std::string::npos)
		{
			delete[] buff;
			return false;
		}
		count = recv(Iter->_client_fd, buff, count, 0); // Читаем до CRLF последовательности
		if (count == -1) // если пришло только число
			return false;
		buff[count] = '\0';
		checker = buff;
		delete[] buff;
		tmp << checker; // Переводим из HEX в DEC
		tmp >> std::hex >> count;
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
			Iter->_chunked_end_check = "";
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
	buff = new char[Iter->_chunked_length + 1];
	count = recv(Iter->_client_fd, buff, Iter->_chunked_length, 0); // Считываем колличесво байт, которое было указано в первом блоке
	if (count == -1)
	{
		delete[] buff;
		return false;
	}
	buff[count] = '\0';
	tmp_chunked_length = Iter->_chunked_length;
	Iter->_chunked_length -= count;
	Iter->_request_body.append(buff);
	delete[] buff;
	if (count < tmp_chunked_length)
		return false;
	count = recv(Iter->_client_fd, crlf_buffer, 2, MSG_PEEK);
	if (count == -1 || std::strncmp(crlf_buffer, "\r\n", 2) != 0)
		return false;
	if (!std::strncmp(crlf_buffer, "\r\n", 2))
	{
		recv(Iter->_client_fd, crlf_buffer, 2, 0);
		Iter->_chunked_length_status = false;
		Iter->_chunked_length = 0;
	}
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
	std::cout << GREEN << output << RESET << std::endl;
	Iter->setClientHandler(new Parse_request_headers(output, Iter->_server_ip, Iter->_client_ip));
	++Iter->_curent_progress;
	delete [] output;
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
