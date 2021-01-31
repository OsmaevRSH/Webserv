#include "Server.hpp"

Server::Server(const serv_vec &serv, const errp_map &err, const ew_str &ew, MIME_ERROR &mime, char **env, int family, int type, int protocol)
		: _config(serv, err, ew), _type(type), _family(family), _protocol(protocol), _readfds(), _writefds(), _mime(mime), env(env) {}

Server::~Server() {}

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
	socklen_t addr_len;
	int new_client_fd;
	addr_len = sizeof(addr);
	new_client_fd = accept(fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	std::cout << "Client №" << client_num << " Client fd: " << new_client_fd << std::endl;
	++client_num;
	if (new_client_fd == -1)
		return;
	inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
	client._client_fd = new_client_fd;
	client._server_client_ip = str;
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
				write_max = Iter->_client_fd > write_max && Iter->_answer_is_ready ?
						Iter->_client_fd : write_max;
	}
	if (!_master_socket_fd.empty())
		for (Iter_1 = _master_socket_fd.begin(); Iter_1 != _master_socket_fd.end() ; ++Iter_1)
			master_max = *Iter_1 > master_max ? *Iter_1 : master_max;
	max_fd = std::max(read_max, write_max);
	max_fd = std::max(max_fd, master_max);
}

void Server::Checkout_call_to_select(const int &res)
{
	if (res <= 0)
	{
		if (errno == EINTR)
		{
			perror("Select error");
			exit(1);
		}
		else if (errno == EBADF)
		{
			perror("Select error");
			exit(2);
		}
		else if (errno == EINVAL)
		{
			perror("Select error");
			exit(3);
		}
		else if (errno == ENOMEM)
		{
			perror("Select error");
			exit(4);
		}
		else
			exit(0);
	}
}

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