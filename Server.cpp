#include "Server.hpp"

Server::Server(int bind_port, int family, int type, int protocol)
		: _bind_port(bind_port), _family(family), _type(type), _protocol(protocol), _socket_fd(0), _client_socket_fd(0)
{
}

Server::Server(const Server &copy)
		: _bind_port(copy._bind_port), _family(copy._family), _type(copy._type), _protocol(copy._protocol),
		_socket_fd(copy._socket_fd), _client_socket_fd(copy._client_socket_fd)
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
	_socket_fd = copy._socket_fd;
	_client_socket_fd = copy._client_socket_fd;
	return *this;
}

void Server::Socket()
{
	_socket_fd = socket(_family, _type, _protocol);
	if (_socket_fd == -1) {
		perror("Create socket error");
		exit(EXIT_FAILURE);
	}
}

void Server::Bind()
{
	int bind_res;
	struct sockaddr_in addr = {0};

	addr.sin_family = _family;
	addr.sin_port = htons(_bind_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind_res = bind(_socket_fd, (struct sockaddr *) (&addr), sizeof(addr));
	if (bind_res == -1) {
		perror("Create bind error");
		exit(EXIT_FAILURE);
	}
}

void Server::Listen()
{
	listen(_socket_fd, 16);
}

void Server::Accept()
{
	struct sockaddr_in addr = {0};
	socklen_t addr_len;

	addr_len = sizeof(reinterpret_cast<struct sockaddr *>(&addr));

	_client_socket_fd = accept(_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	if (_client_socket_fd == -1) {
		perror("Accept error");
		exit(EXIT_FAILURE);
	}
}
void Server::server_start()
{
	this->Socket();
	this->Bind();
	this->Listen();
	this->Accept();
}
