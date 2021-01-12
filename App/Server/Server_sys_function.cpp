#include "Server.hpp"

Server::Server(const std::vector<ConfigParser::t_server> &servers_config, Config &config, int family, int type, int protocol)
		: _family(family), _type(type), _protocol(protocol), _readfds(),
		_writefds(), _master_socket_fd(0), _servers_config(servers_config),
		_config(config)
{
}

Server::Server(const Server &copy)
		: _family(copy._family), _type(copy._type), _protocol(copy._protocol),
		_readfds(), _writefds(), _master_socket_fd(copy._master_socket_fd),
		_read_socket_fd(copy._read_socket_fd),
		_servers_config(copy._servers_config), _config(copy._config)
{
}

Server::~Server()
{
}

Server &Server::operator=(const Server &copy)
{
	_family = copy._family;
	_type = copy._type;
	_protocol = copy._protocol;
	_master_socket_fd = copy._master_socket_fd;
	_read_socket_fd = copy._read_socket_fd;
	_servers_config = copy._servers_config;
	_config = copy._config;
	return *this;
}

void Server::Socket()
{
	_master_socket_fd.reserve(_servers_config.size());
	for (int i = 0; i < static_cast<int>(_servers_config.size()); ++i)
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
	Iter = _servers_config.begin();
	Iter_fd = _master_socket_fd.begin();
#ifdef SERVER_IP_DEBUG
	int i = 0;
#endif
	int bind_res;
	struct sockaddr_in addr = {};

	for (; Iter < _servers_config.end(); ++Iter, ++Iter_fd)
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
		std::cout << "Server[" << i++ + 1 << "]:[" << Iter->ip << ":"
				  << Iter->port << "]" << std::endl;
#endif
	}
}

void Server::Listen() const
{
	std::vector<int>::const_iterator it;
	it = _master_socket_fd.begin();

	for (; it < _master_socket_fd.end(); ++it)
	{
		listen(*it, 16);
	}
}

void Server::Accept(int fd)
{
	struct sockaddr_in addr = {0};
	socklen_t addr_len;
	int new_client_fd;
	addr_len = sizeof(addr);
	new_client_fd = accept(fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	if (new_client_fd == -1)
	{
		perror("Accept error");
		exit(EXIT_FAILURE);
	}
	Set_non_blocked(new_client_fd);
	_read_socket_fd.push_back(new_client_fd);
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
	std::vector<int>::iterator Iter;

	for (Iter = _master_socket_fd.begin();
		 Iter < _master_socket_fd.end(); ++Iter)
		FD_SET(*Iter, &_readfds);
	for (Iter = _read_socket_fd.begin(); Iter != _read_socket_fd.end(); ++Iter)
		FD_SET(*Iter, &_readfds);
	for (Iter = _write_socket_fd.begin();
		 Iter != _write_socket_fd.end(); ++Iter)
		FD_SET(*Iter, &_writefds);
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
	int read_max = 0;
	int write_max = 0;
	int master_max = 0;

	if (!_read_socket_fd.empty())
		read_max = *(std::max_element(_read_socket_fd.begin(), _read_socket_fd.end()));
	if (!_write_socket_fd.empty())
		write_max = *(std::max_element(_write_socket_fd.begin(), _write_socket_fd.end()));
	if (!_master_socket_fd.empty())
		master_max = *(std::max_element(_master_socket_fd.begin(), _master_socket_fd.end()));
	max_fd = std::max(read_max, write_max);
	max_fd = std::max(max_fd, master_max);
}

void Server::Checkout_call_to_select(const int &res)
{
	if (res <= 0)
	{
		if (errno != EINTR)
		{
			perror("Select error");
			exit(1);
		}
		else
			exit(0);
	}
}

void Server::Check_read_set()
{
	std::vector<int>::iterator Iter;

	Iter = _read_socket_fd.begin();
	while (Iter != _read_socket_fd.end())
	{
		if (FD_ISSET(*Iter, &_readfds))
			Act_if_readfd_changed(Iter);
		else
			++Iter;
	}
}

void Server::Check_write_set()
{
	std::vector<int>::iterator Iter;

	Iter = _write_socket_fd.begin();
	while (Iter != _write_socket_fd.end())
	{
		if (FD_ISSET(*Iter, &_writefds))
			Act_if_writefd_changed(Iter);
		else
			++Iter;
	}
}

Input_handlers *Server::Reading_a_request(std::vector<int>::iterator &Iter)
{
	char *buffer_for_request;
	char *output;
	int request_size;
	Input_handlers *inputHandlers;

	buffer_for_request = new char[576];
	request_size = recv(*Iter, buffer_for_request, 575, 0);
	if (request_size == 0 &&
		_request_to_client.find(*Iter) == _request_to_client.end())
	{
		close(*Iter);
		Iter = _read_socket_fd.erase(Iter);
		delete[] buffer_for_request;
		return nullptr;
	}
	if (request_size > 0)
		buffer_for_request[request_size] = '\0';
	if (request_size == -1 ||
		(output = check_input_handler_buffer(buffer_for_request, Iter)) ==
		nullptr)
	{
		++Iter;
		delete[] buffer_for_request;
		return nullptr;
	}
	inputHandlers = new Input_handlers(output);
	if ((inputHandlers->getVariableHandlers().find("Connection") !=
		 inputHandlers->getVariableHandlers().end() &&
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