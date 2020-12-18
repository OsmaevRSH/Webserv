#include "Server.hpp"

Server::Server(int bind_port, int family, int type, int protocol)
		: _bind_port(bind_port), _family(family), _type(type),
		_protocol(protocol), _master_socket_fd(0)
{
}

Server::Server(const Server &copy)
		: _bind_port(copy._bind_port), _family(copy._family), _type(copy._type),
		_protocol(copy._protocol), _master_socket_fd(copy._master_socket_fd),
		_client_socket_fd(copy._client_socket_fd)
{
}

Server::~Server()
{
}

Server &Server::operator=(const Server &copy)
{
	_bind_port = copy._bind_port;
	_family = copy._family;
	_type = copy._type;
	_protocol = copy._protocol;
	_master_socket_fd = copy._master_socket_fd;
	_client_socket_fd = copy._client_socket_fd;
	return *this;
}

void Server::Socket()
{
	_master_socket_fd = socket(_family, _type, _protocol);
	if (_master_socket_fd == -1) {
		perror("Create socket error");
		exit(EXIT_FAILURE);
	}
}

void Server::Bind()
{
	int bind_res;
	struct sockaddr_in addr = {0}; //netinet/in.h

	addr.sin_family = _family;
	addr.sin_port = htons(_bind_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("0.0.0.0")

	int opt = 1;
	setsockopt(_master_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bind_res = bind(_master_socket_fd, (struct sockaddr *) (&addr), sizeof(addr));
	if (bind_res == -1) {
		perror("Create bind error");
		exit(EXIT_FAILURE);
	}
}

void Server::Listen() const
{
	listen(_master_socket_fd, 16);
}

void Server::Accept()
{
	struct sockaddr_in addr = {0};
	socklen_t addr_len;
	int new_client_fd;

	addr_len = sizeof(addr);

	new_client_fd = accept(_master_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	if (new_client_fd == -1) {
		perror("Accept error");
		exit(EXIT_FAILURE);
	}
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
	int max_fd;
	fd_set readfds, writefds;
	std::vector<int>::iterator Iter;

	while (true) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(_master_socket_fd, &readfds);
		for (Iter = _client_socket_fd.begin();
			 Iter != _client_socket_fd.end(); ++Iter) {
			FD_SET(*Iter, &readfds);
		}
		if (!_client_socket_fd.empty()) {
			max_fd = *(std::max_element(_client_socket_fd.begin(), _client_socket_fd.end()));
		}
		max_fd = max_fd > _master_socket_fd ? max_fd : _master_socket_fd;

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
		if (FD_ISSET(_master_socket_fd, &readfds)) {
			Accept();
		}
		Iter = _client_socket_fd.begin();
		while (Iter != _client_socket_fd.end()) {
			if (FD_ISSET(*Iter, &readfds)) {
				char *buf = (char *) malloc(sizeof(char) * 576);
				std::cout << "Wait..." << std::endl;
				if ((recv(*Iter, buf, 576, 0)) == 0) {
					close(*Iter);
					std::cout << "Close connection... " << *Iter << std::endl;
					Iter = _client_socket_fd.erase(Iter);
				}
				else {
//					std::cout << buf << std::endl;
					send(*Iter, ("HTTP/1.1 200 OK\nContent-type: text/html\n\n" + get_page_text("index.html")).c_str(), 576, 0);
					shutdown(*Iter, SHUT_RDWR);
//					close(*Iter);
					std::cout << "Send and close connection... " << *Iter << std::endl;
					Iter = _client_socket_fd.erase(Iter);
//					++Iter;
				}
			}
			else {
				std::cout << "Skip connection... " << *Iter << std::endl;
				++Iter;
			}
		}
	}
}
