#include "Server.hpp"

//Конструктор
Server::Server(const std::vector<ConfigParser::t_server> &servers_config, Config &config, int family, int type, int protocol)
		: _family(family), _type(type), _protocol(protocol),
		_master_socket_fd(0), _servers_config(servers_config), _config(config)
{
}

//Конструктор копирования
Server::Server(const Server &copy)
		: _family(copy._family), _type(copy._type), _protocol(copy._protocol),
		_master_socket_fd(copy._master_socket_fd),
		_read_socket_fd(copy._read_socket_fd),
		_servers_config(copy._servers_config), _config(copy._config)
{
}

//Деструктор
Server::~Server()
{
}

//Оператор =
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

//Обертка для системной функции socket
void Server::Socket()
{
	_master_socket_fd.reserve(_servers_config.size()); //резервируем необходимое место в векторе в завтисимотсти от кол-ва серверов
	for (int i = 0; i < _servers_config.size(); ++i)
	{
		_master_socket_fd.push_back(socket(_family, _type, _protocol)); //создем дескрипторы для нашего сервера
		if (*_master_socket_fd.rbegin() == -1)
		{
			perror("Create socket error");
			exit(EXIT_FAILURE);
		}
		Set_non_blocked(*_master_socket_fd.rbegin()); // переводим дескрипотры в неблокирующий режим
	}
}

//Обертка для системной функции bind
void Server::Bind()
{
	std::vector<ConfigParser::s_server>::iterator Iter;
	std::vector<int>::iterator Iter_fd;
	Iter = _servers_config.begin();
	Iter_fd = _master_socket_fd.begin();
#ifdef SERVER_IP_DEBUG
	int i = 0;
#endif

	int bind_res; // переменная для хранения возваращаемого значения функции bind
	struct sockaddr_in addr = {};

	for (; Iter < _servers_config.end(); ++Iter, ++Iter_fd)
	{
		bzero(&addr, sizeof(addr));
		addr.sin_family = _family;
		if (Iter->port == 0)
			Iter->port = 8080;
		if (Iter->ip.empty())
			Iter->ip = "0.0.0.0";
		addr.sin_port = htons(Iter->port); //порт сервера
		addr.sin_addr.s_addr = inet_addr(Iter->ip.c_str()); //IP адрес сервера

		int opt = 1;
		setsockopt(*Iter_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //Разрешение повторного использования порт+IP для сервера

		bind_res = bind(*Iter_fd, (struct sockaddr *) (&addr), sizeof(addr)); //Вызов bind теперь дескриптор сервера привязан в порт+IP
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

//Обертка для системной функции listen
void Server::Listen() const
{
	std::vector<int>::const_iterator it;
	it = _master_socket_fd.begin();

	for (; it < _master_socket_fd.end(); ++it)
	{
		listen(*it, 16); //Переводим все дескрипторы сервера в слушающий режим
	}
}

//Обертка для системной функции accept
void Server::Accept(int fd)
{
	struct sockaddr_in addr = {};
	socklen_t addr_len; //переменая для хранения длины структуры
	int new_client_fd; //тут будет созранени дескрипотор нового клиента

	addr_len = sizeof(addr);

	new_client_fd = accept(fd, reinterpret_cast<struct sockaddr *>(&addr), &addr_len); //возвращение дексриптор нового клиента
	if (new_client_fd == -1)
	{
		perror("Accept error");
		exit(EXIT_FAILURE);
	}
	Set_non_blocked(new_client_fd); //Перевод дескриптора в неблокирующий режим
	_read_socket_fd.push_back(new_client_fd); //Добавляем нового клиента в список всех клиентов
}

//Функиция для старта всего сервера
void Server::server_start()
{
	this->Socket();
	this->Bind();
	this->Listen();
	this->ListenLoop();
}


//Основаной цикл где происходит обработка соединений
_Noreturn void Server::ListenLoop()
{
	int max_fd = 0;
	std::vector<int>::iterator Iter;

	while (true)
	{
		Reset_fd_set();
		Add_new_fd_to_set();
		Search_max_fd(max_fd);
		int res = select(max_fd + 1, &_readfds, nullptr, nullptr, nullptr);
		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed();
		//		Iter = _write_socket_fd.begin();
		//		while (Iter != _write_socket_fd.end())
		//		{
		//			if (FD_ISSET(*Iter, &_writefds))
		//			{ //проверяем, выставлен ли какой-то бит связанный с клиентский дескрипотором
		//				Act_if_writefd_changed(Iter);
		//			}
		//			else
		//				++Iter;
		//		}
		Iter = _read_socket_fd.begin();
		while (Iter != _read_socket_fd.end())
		{
			if (FD_ISSET(*Iter, &_readfds))
				Act_if_readfd_changed(Iter);
			else
				++Iter;
		}
	}
}

void Server::Act_if_writefd_changed(std::vector<int>::iterator &Iter)
{
	int send_number;
	send_number = send(*Iter, (_request_to_client[*Iter][0] +
							   _request_to_client[*Iter][1]).c_str(),
			_request_to_client[*Iter][0].size() +
			_request_to_client[*Iter][1].size(), 0);
	if (send_number < _request_to_client[*Iter][0].size() +
					  _request_to_client[*Iter][1].size())
	{
		if (send_number <= _request_to_client[*Iter][0].size())
			_request_to_client[*Iter][0].erase(0, send_number);
		else
		{
			_request_to_client[*Iter][1].erase(0,
					send_number - _request_to_client[*Iter][0].size());
			_request_to_client[*Iter][0].erase(_request_to_client[*Iter][0].begin(), _request_to_client[*Iter][0].end());
		}
	}
	else
		Iter = _write_socket_fd.erase(Iter);
}

//Функция перевода дескриптора в неблокирующий режим
void Server::Set_non_blocked(int fd)
{
	int flags;
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Server::Reset_fd_set()
{
	FD_ZERO(&_readfds); //обнулем все биты в сете
	FD_ZERO(&_writefds); //обнулем все биты в сете
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

bool Server::Checkout_call_to_select(const int &res)
{
	if (res < 1)
	{
		if (errno != EINTR)
		{
			perror("Select error");
			exit(1);
		}
		else
			exit(0);
	}
	if (res == 0)
		return true;
	return false;
}

void Server::Accept_if_serv_fd_changed()
{
	std::vector<int>::iterator it;
	it = _master_socket_fd.begin();
	for (; it < _master_socket_fd.end(); ++it)
	{
		if (FD_ISSET(*it, &_readfds))
		{ //проверяем был ли выставлен бит серверного дескриптра, если да, то создаем новое подключение
			Accept(*it);
		}
	}
}

void Server::Act_if_readfd_changed(std::vector<int>::iterator &Iter)
{
	int recv_res;
	char *buf = (char *) malloc(sizeof(char) * 576); //создаем буфер для чтения
	recv_res = recv(*Iter, buf, 576, 0);
	Input_handlers inputHandlers(buf);
#ifdef SERVER_DEBUG
	inputHandlers.output();
#endif
	if ((recv_res == 0 &&
		 _request_to_client.find(*Iter) == _request_to_client.end()) ||
		(inputHandlers.getVariableHandlers().find("Connection") !=
		 inputHandlers.getVariableHandlers().end() &&
		 inputHandlers.getVariableHandlers().at("Connection") == "close"))
	{
		shutdown(*Iter, SHUT_RDWR); //разрыв соединения
		close(*Iter); //закрытие дескриптора
		Iter = _read_socket_fd.erase(Iter); //удаление дескриптора из пула клиентских дескрипторов
	}
	std::string file = _config.Handler(inputHandlers.getHandlers(), inputHandlers);
	//	std::vector<std::string> tmp;
	//	tmp.push_back(
	//			"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " +
	//			std::to_string(file.size()) + "\r\n\r\n");
	//	tmp.push_back(file);
	//	_request_to_client.insert(std::pair<int, std::vector<std::string> >(*Iter, tmp));
	//	_write_socket_fd.push_back(*Iter);
	send(*Iter, (
			"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " +
			std::to_string(file.size()) + "\r\n\r\n" + file).c_str(),
			65 + file.size(), 0); // передача вообщения клиенту
	shutdown(*Iter, SHUT_WR); //разрыв соединения на передачу
	close(*Iter); //закрытие клиентского дескриптора
	Iter = _read_socket_fd.erase(Iter); //удаление дескриптора из пула клиентских дескрипторов
}