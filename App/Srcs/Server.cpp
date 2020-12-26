#include "Server.hpp"

//Конструктор
Server::Server(const std::vector<t_server> &servers_config, int family, int type, int protocol)
		: _family(family), _type(type), _protocol(protocol),
		_master_socket_fd(0), _servers_config(servers_config),
		_count_servers(servers_config.size())
{
}

//Конструктор копирования
Server::Server(const Server &copy)
		: _family(copy._family), _type(copy._type), _protocol(copy._protocol),
		_master_socket_fd(copy._master_socket_fd),
		_client_socket_fd(copy._client_socket_fd),
		_servers_config(copy._servers_config),
		_count_servers(copy._count_servers)
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
	_client_socket_fd = copy._client_socket_fd;
	_servers_config = copy._servers_config;
	_count_servers = copy._count_servers;
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
		addr.sin_port = htons(_servers_config[i].port); //порт сервера
		addr.sin_addr.s_addr = inet_addr((_servers_config[i].ip).c_str()); //IP адрес сервера

		int opt = 1;
		setsockopt(_master_socket_fd[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //Разрешение повторного использования порт+IP для сервера

		bind_res = bind(_master_socket_fd[i], (struct sockaddr *) (&addr), sizeof(addr)); //Вызов bind теперь дескриптор сервера привязан в порт+IP
		if (bind_res == -1)
		{
			perror("Create bind error");
			exit(EXIT_FAILURE);
		}
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
	struct sockaddr_in addr = {0};
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
	// std::cout << "New client connect... " << new_client_fd << std::endl;
	_client_socket_fd.push_back(new_client_fd); //Добавляем нового клиента в список всех клиентов
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
	fd_set readfds, writefds; //создем сет для чтения и записи
	std::vector<int>::iterator Iter;

	while (true)
	{
		Reset_fd_set(readfds, writefds);
		Add_new_fd_to_set(readfds, Iter);
		Search_max_fd(max_fd);
		int res = select(max_fd +
						 1, &readfds, nullptr, nullptr, nullptr); //отвлеживает состояние дескрипторов и выставляет в 1 бит дескриптора если с него можно читать или писать
		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed(readfds);
		Iter = _client_socket_fd.begin();
		while (Iter != _client_socket_fd.end())
		{
			if (FD_ISSET(*Iter, &readfds))
			{ //проверяем, выставлен ли какой-то бит связанный с клиентский дескрипотором
				Act_if_client_fd_changed(Iter);
			}
			else
			{
				++Iter;
			}
		}
	}
}


//Функция перевода дескриптора в неблокирующий режим
void Server::Set_non_blocked(int fd)
{
	int flags;
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
	{
		flags = 0;
	}
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Server::Reset_fd_set(fd_set &readfds, fd_set &writefds)
{
	FD_ZERO(&readfds); //обнулем все биты в сете
	FD_ZERO(&writefds); //обнулем все биты в сете
}

void Server::Add_new_fd_to_set(fd_set &readfds, std::vector<int>::iterator Iter)
{
	for (int i = 0; i < _count_servers; ++i)
	{
		FD_SET(_master_socket_fd[i], &readfds); //добавляем серверные дескрипторы в сет
	}
	for (Iter = _client_socket_fd.begin();
		 Iter != _client_socket_fd.end(); ++Iter)
		FD_SET(*Iter, &readfds); //добавляем клиентсикие дескрипторы в сет
}

void Server::Search_max_fd(int &max_fd)
{
	if (!_client_socket_fd.empty())
		max_fd = *(std::max_element(_client_socket_fd.begin(), _client_socket_fd.end())); //находим максимальный дескриптор среди клиентский
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
		{
			exit(0);
		}
	}
	if (res == 0)
	{
		return true;
	}
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

void Server::Act_if_client_fd_changed(std::vector<int>::iterator &Iter)
{
	char *buf = (char *) malloc(sizeof(char) * 576); //создаем буфер для чтения
	if ((recv(*Iter, buf, 576, 0)) == 0)
	{ //читаем и в случае если пришло пустое сообщение заходим в if и инициируем разрый соединения
		shutdown(*Iter, SHUT_RDWR); //разрыв соединения
		close(*Iter); //закрытие дескриптора
		// std::cout << "Close connection... " << *Iter << std::endl;
		Iter = _client_socket_fd.erase(Iter); //удаление дескриптора из пула клиентских дескрипторов
	}
	else
	{
		Input_handlers inputHandlers(buf);
#ifdef DEBUG
		inputHandlers.output();
#endif
		shutdown(*Iter, SHUT_RD); //разрый соединенеия на чтение
		std::string file = get_page_text("index.html");
		send(*Iter, ("HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n" +
					 file).c_str(),
				44 + file.size(), 0); // передача вообщения клиенту
		shutdown(*Iter, SHUT_WR); //разрыв соединения на передачу
		close(*Iter); //закрытие клиентского дескриптора
		// std::cout << "Send and close connection... " << *Iter << std::endl;
		Iter = _client_socket_fd.erase(Iter); //удаление дескриптора из пула клиентских дескрипторов
	}
}