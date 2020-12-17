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
	write(_client_socket_fd, "HTTP/1.1 200 OK\nContent-type: text/html\n\n<!DOCTYPE html>\n"
							 "<html>\n"
							 "<head>\n"
							 "    <title>Welcome to nginx!</title>\n"
							 "    <style>\n"
							 "        body {\n"
							 "            width: 35em;\n"
							 "            margin: 0 auto;\n"
							 "            font-family: Tahoma, Verdana, Arial, sans-serif;\n"
							 "        }\n"
							 "    </style>\n"
							 "</head>\n"
							 "<body>\n"
							 "<h1>Welcome to nginx!</h1>\n"
							 "<p>If you see this page, the nginx web server is successfully installed and\n"
							 "    working. Further configuration is required.</p>\n"
							 "\n"
							 "<p>For online documentation and support please refer to\n"
							 "    <a href=\"http://nginx.org/\">nginx.org</a>.<br/>\n"
							 "    Commercial support is available at\n"
							 "    <a href=\"http://nginx.com/\">nginx.com</a>.</p>\n"
							 "\n"
							 "<p><em>Thank you for using nginx.</em></p>\n"
							 "</body>\n"
							 "</html>", 500);
	close(_client_socket_fd);
	}
}
void Server::server_start()
{
	this->Socket();
	this->Bind();
	this->Listen();
	this->Accept();
}
