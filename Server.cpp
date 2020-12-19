#include "Server.hpp"

Server::Server(const std::vector<std::map<std::string, std::string> > &servers_config,
			int family, int type, int protocol)
		:	_family(family),
			_type(type),
			_protocol(protocol),
			_master_socket_fd(0),
			_servers_config(servers_config),
			_count_servers(servers_config.size())
{
}

Server::Server(const Server &copy)
		:	_family(copy._family),
			_type(copy._type),
			_protocol(copy._protocol),
			_master_socket_fd(copy._master_socket_fd),
			_client_socket_fd(copy._client_socket_fd),
			_servers_config(copy._servers_config),
			_count_servers(copy._count_servers)
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
	_client_socket_fd = copy._client_socket_fd;
	_servers_config = copy._servers_config;
	_count_servers =copy._count_servers;
	return *this;
}

void Server::Socket()
{
	_master_socket_fd.reserve(_servers_config.size());
	for (int i = 0; i < _count_servers; ++i) {
		_master_socket_fd[i] = socket(_family, _type, _protocol);
		if (_master_socket_fd[i] == -1) {
			perror("Create socket error");
			exit(EXIT_FAILURE);
		}
		setNonBlocked(_master_socket_fd[i]);
	}
}

void Server::Bind()
{
	int bind_res;
	struct sockaddr_in addr = {};

	for (int i = 0; i < _count_servers; ++i) {
		bzero(&addr, sizeof(addr));
		addr.sin_family = _family;
		addr.sin_port = htons(std::stoi(_servers_config[i]["port"]));
		addr.sin_addr.s_addr = inet_addr((_servers_config[i]["address"]).c_str()); //inet_addr("0.0.0.0")

		int opt = 1;
		setsockopt(_master_socket_fd[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		bind_res = bind(_master_socket_fd[i], (struct sockaddr *) (&addr), sizeof(addr));
		if (bind_res == -1) {
			perror("Create bind error");
			exit(EXIT_FAILURE);
		}
	}
}

void Server::Listen() const
{
	for (int i = 0; i < _count_servers; ++i) {
		listen(_master_socket_fd[i], 16);
	}
}

void Server::Accept(int fd)
{
	struct sockaddr_in addr = {0};
	socklen_t addr_len;
	int new_client_fd;

	addr_len = sizeof(addr);

	new_client_fd = accept(fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	if (new_client_fd == -1) {
		perror("Accept error");
		exit(EXIT_FAILURE);
	}
	setNonBlocked(new_client_fd);
	std::cout << "New client connect... " << new_client_fd << std::endl;
	_client_socket_fd.push_back(new_client_fd);
}
void Server::server_start()
{
	this->Socket();
	this->Bind();
	this->Listen();
	this->ListenLoop();
}

void Server::ListenLoop()
{
	int max_fd = 0;
	fd_set readfds, writefds;
	std::vector<int>::iterator Iter;

	while (true) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		for (int i = 0; i < _count_servers; ++i) {
			FD_SET(_master_socket_fd[i], &readfds);
		}
		for (Iter = _client_socket_fd.begin();
			 Iter != _client_socket_fd.end(); ++Iter) {
			FD_SET(*Iter, &readfds);
		}
		if (!_client_socket_fd.empty()) {
			max_fd = *(std::max_element(_client_socket_fd.begin(), _client_socket_fd.end()));
		}
		for (int i = 0; i < _count_servers; ++i) {
			if (_master_socket_fd[i] > max_fd) {
				max_fd = _master_socket_fd[i];
			}
		}
		int res = select(max_fd + 1, &readfds, nullptr, nullptr, nullptr);
		if (res < 1) {
			if (errno != EINTR) {
				perror("Select error");
				exit(1);
			}
			else {
				exit(0);
			}
		}
		if (res == 0) {
			continue;
		}
		for (int i = 0; i < _count_servers; ++i) {
			if (FD_ISSET(_master_socket_fd[i], &readfds)) {
				Accept(_master_socket_fd[i]);
			}
		}
		Iter = _client_socket_fd.begin();
		while (Iter != _client_socket_fd.end()) {
			if (FD_ISSET(*Iter, &readfds)) {
				char *buf = (char *) malloc(sizeof(char) * 576);
				if ((recv(*Iter, buf, 576, 0)) == 0) {
					shutdown(*Iter, SHUT_RDWR);
					close(*Iter);
					std::cout << "Close connection... " << *Iter << std::endl;
					Iter = _client_socket_fd.erase(Iter);
				}
				else {
					shutdown(*Iter, SHUT_RD);
					send(*Iter, (
							"HTTP/1.1 200 OK\nContent-type: text/html\n\n" +
							get_page_text("index.html")).c_str(), 576, 0);
					shutdown(*Iter, SHUT_WR);
					close(*Iter);
					std::cout << "Send and close connection... " << *Iter
							  << std::endl;
					Iter = _client_socket_fd.erase(Iter);
				}
			}
			else {
				++Iter;
			}
		}
	}
}

void Server::setNonBlocked(int fd)
{
	int flags;
	if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
		flags = 0;
	}
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
