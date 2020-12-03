#include "Server.hpp"

Server::Server(int family, int type, int protocol)
	: _family(family), _type(type), _protocol(protocol), _socket_fd(0)
{
}
Server::Server(const Server &copy)
	: _family(copy._family), _type(copy._type), _protocol(copy._protocol),
	  _socket_fd(copy._socket_fd)
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
	_socket_fd = copy._socket_fd;
	return *this;
}
void Server::Socket()
{
	_socket_fd = socket(_family, _type, _protocol);
	if (_socket_fd == -1)
	{
		perror("Create socket error:\n");
		exit(EXIT_FAILURE);
	}
}

void Server::Bind()
{
	int bind_res;
	struct sockaddr_in *addr = nullptr;

	addr->sin_family = _family;
	addr->sin_port = htons(7654);

	bind_res = bind(_socket_fd,
					reinterpret_cast<struct sockaddr *>(addr),
					sizeof(reinterpret_cast<struct sockaddr *>(addr)));
	if (bind_res == -1)
	{
		perror("Create bind error:\n");
		exit(EXIT_FAILURE);
	}
}
