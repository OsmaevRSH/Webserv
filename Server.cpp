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
	struct sockaddr_in addr = {0}; //netinet/in.h

	addr.sin_family = _family;
	addr.sin_port = htons(_bind_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("0.0.0.0")

	int opt = 1;
	setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bind_res = bind(_socket_fd, (struct sockaddr *)(&addr), sizeof(addr));
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

	addr_len = sizeof(addr);

	while (true)
	{
	_client_socket_fd = accept(_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len);
	char *test = (char *)malloc(sizeof(char) * 100000000);
	if (_client_socket_fd == -1) {
		perror("Accept error");
		exit(EXIT_FAILURE);
	}
	read(_client_socket_fd, test, 100000000);
	std::cout << test;
	//проврека на наличие и доступ к файлу
	std::string html = get_page_text("./index.html"); //парсер html
	write(_client_socket_fd, ("HTTP/1.1 200 OK\nContent-type: text/html\n\n" + html).c_str(), 44 + html.size());
//	close(_client_socket_fd);
//	shutdown(_client_socket_fd, SHUT_WR);
	}
}
void Server::server_start()
{
	this->Socket();
	this->Bind();
	this->Listen();
	this->Accept();
}
