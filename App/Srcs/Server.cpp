#include "Server.hpp"

//Конструктор
Server::Server(const std::vector<ConfigParser::t_server> &servers_config, Config &config, int family, int type, int protocol)
		: _family(family), _type(type), _protocol(protocol),
		_master_socket_fd(0), _servers_config(servers_config),
		_count_servers(servers_config.size()), _config(config)
{
}

//Конструктор копирования
Server::Server(const Server &copy)
		: _family(copy._family), _type(copy._type), _protocol(copy._protocol),
		_master_socket_fd(copy._master_socket_fd),
		_read_socket_fd(copy._read_socket_fd),
		_servers_config(copy._servers_config),
		_count_servers(copy._count_servers), _config(copy._config)
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
	_count_servers = copy._count_servers;
	_config = copy._config;
	return *this;
}

//Обертка для системной функции socket
void Server::Socket()
{
	_master_socket_fd.reserve(_servers_config.size()); //резервируем необходимое место в векторе в завтисимотсти от кол-ва серверов
	for (int i = 0; i < _count_servers; ++i)
	{
		_master_socket_fd[i] = socket(_family, _type, _protocol); //создем дескрипторы для нашего сервера
		if (_master_socket_fd[i] == -1)
		{
			perror("Create socket error");
			exit(EXIT_FAILURE);
		}
		Set_non_blocked(_master_socket_fd[i]); // переводим дескрипотры в неблокирующий режим
	}
}

//Обертка для системной функции bind
void Server::Bind()
{
	int bind_res; // переменная для хранения возваращаемого значения функции bind
	struct sockaddr_in addr = {};

	for (int i = 0; i < _count_servers; ++i)
	{
		bzero(&addr, sizeof(addr));
		addr.sin_family = _family;
		if (_servers_config[i].port == 0)
			_servers_config[i].port = 8080;
		if (_servers_config[i].ip.empty())
			_servers_config[i].ip = "0.0.0.0";
		addr.sin_port = htons(_servers_config[i].port); //порт сервера
		addr.sin_addr.s_addr = inet_addr(_servers_config[i].ip.c_str()); //IP адрес сервера

		int opt = 1;
		setsockopt(_master_socket_fd[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //Разрешение повторного использования порт+IP для сервера

		bind_res = bind(_master_socket_fd[i], (struct sockaddr *) (&addr), sizeof(addr)); //Вызов bind теперь дескриптор сервера привязан в порт+IP
		if (bind_res == -1)
		{
			perror("Create bind error");
			exit(EXIT_FAILURE);
		}
#ifdef SERVER_IP_DEBUG
		std::cout << "Server[" << i + 1 << "]:[" << _servers_config[i].ip << ":"
				  << _servers_config[i].port << "]" << std::endl;
#endif
	}
}

//Обертка для системной функции listen
void Server::Listen() const
{
	for (int i = 0; i < _count_servers; ++i)
	{
		listen(_master_socket_fd[i], 16); //Переводим все дескрипторы сервера в слушающий режим
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
		Add_new_fd_to_set(Iter);
		Search_max_fd(max_fd);
		int res = select(max_fd +
						 1, &_readfds, &_writefds, nullptr, nullptr); //отвлеживает состояние дескрипторов и выставляет в 1 бит дескриптора если с него можно читать или писать
		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed(_readfds);
		Iter = _write_socket_fd.begin();
		while (Iter != _write_socket_fd.end())
		{
			if (FD_ISSET(*Iter, &_writefds))
			{ //проверяем, выставлен ли какой-то бит связанный с клиентский дескрипотором
				Act_if_writefd_changed(Iter);
			}
			else
				++Iter;
		}
		Iter = _read_socket_fd.begin();
		while (Iter != _read_socket_fd.end())
		{
			if (FD_ISSET(*Iter, &_readfds))
			{ //проверяем, выставлен ли какой-то бит связанный с клиентский дескрипотором
				Act_if_readfd_changed(Iter);
			}
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

void Server::Add_new_fd_to_set(std::vector<int>::iterator Iter)
{
	for (int i = 0; i < _count_servers; ++i)
		FD_SET(_master_socket_fd[i], &_readfds); //добавляем серверные дескрипторы в сет
	for (Iter = _read_socket_fd.begin(); Iter != _read_socket_fd.end(); ++Iter)
		FD_SET(*Iter, &_readfds); //добавляем клиентсикие дескрипторы в сет
	for (Iter = _write_socket_fd.begin();
		 Iter != _write_socket_fd.end(); ++Iter)
		FD_SET(*Iter, &_writefds); //добавляем клиентсикие дескрипторы в сет
}

void Server::Search_max_fd(int &max_fd)
{
	if (!_read_socket_fd.empty())
	{
		max_fd = *(std::max_element(_read_socket_fd.begin(), _read_socket_fd.end())); //находим максимальный дескриптор среди клиентский
	}
	if (!_write_socket_fd.empty())
	{
		max_fd =
				*(std::max_element(_write_socket_fd.begin(), _write_socket_fd.end())) >
				max_fd
				? *(std::max_element(_write_socket_fd.begin(), _write_socket_fd.end()))
				: max_fd;
	}
	for (int i = 0; i < _count_servers; ++i)
	{ //проверяем нет ли среди серверных дескрипотора больше, чем максимальный
		if (_master_socket_fd[i] > max_fd)
			max_fd = _master_socket_fd[i];
	}
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

void Server::Accept_if_serv_fd_changed(fd_set &readfds)
{
	for (int i = 0; i < _count_servers; ++i)
	{
		if (FD_ISSET(_master_socket_fd[i], &readfds))
		{ //проверяем был ли выставлен бит серверного дескриптра, если да, то создаем новое подключение
			Accept(_master_socket_fd[i]);
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
	std::vector<std::string> tmp;
	tmp.push_back(
			"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " +
			std::to_string(file.size()) + "\r\n\r\n");
	tmp.push_back(file);
	_request_to_client.insert(std::pair<int, std::vector<std::string> >(*Iter, tmp));
	_write_socket_fd.push_back(*Iter);
	//		send(*Iter, ("HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " + std::to_string(file.size()) + "\r\n\r\n" + file).c_str(),
	//				65 + file.size(), 0); // передача вообщения клиенту
	//		shutdown(*Iter, SHUT_WR); //разрыв соединения на передачу
	//		close(*Iter); //закрытие клиентского дескриптора
	//		Iter = _read_socket_fd.erase(Iter); //удаление дескриптора из пула клиентских дескрипторов
}