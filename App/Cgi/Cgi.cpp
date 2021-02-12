#include "Cgi.h"
#include <fstream>
#include <string>

/*
 * Логика добавления переменных такая:
 * Создаем массив строк с максимальным возможным размером.
 * Мы не знаем, какие переменные у нас могут быть. Для каждой переменной будет
 * своя ячейка в массиве.
 * Если переменно нет, ячейка пуста.
 * Затем, когда будем добавлять их уже в массив переменных окружения, то
 * мы ведем счетчик до кол-ва переменных.
 * Если видим, что данная ячека пуста - просто пропускаем ее.
 * */

Cgi::Cgi(const std::string &path_to_cgi, const t_data_for_cgi &data) : _path_to_cgi(path_to_cgi),
                                                                        _data(data),
                                                                        _env(get_meta_variables(data))
{
	_args[0] = const_cast<char *>(_path_to_cgi.c_str());
	_args[1] = _data.pathToCgiScript;
	_args[2] = nullptr;
	handleRequest();
}

Cgi::~Cgi()
{
	if (_env)
	{
		for (int i = 0; _env[i]; ++i)
			free(_env[i]);
		delete [] _env;
	}
}

void Cgi::handleRequest()
{
	int fd[2], child;

	pipe(fd);
	tmp_fd = open("./.tmp_cgi", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	child = fork();
	if (child == 0)
	{
		close(fd[1]); //ничего не пишем
		dup2(tmp_fd, 1);
		dup2(fd[0], 0);
		close(tmp_fd);
		close(fd[0]);
		if (execve(_args[0], _args, _env) == -1)
			exit(2);
		exit(1);
	}
	else
	{
		close(fd[0]);
		write(fd[1], _data.body.c_str(), _data.body.size());
		close(fd[1]);
		waitpid(child, nullptr, 0);
	}
}

/* Тут нужно определиться с возвращаемым типом. Если тупо копировать строку, то это может быть долго.
 * Но тогда нужно выделять память под сишную строку, и потом не забыть ее отчистить.
 * Хм... Отдам-ка я сишную строку и отчистю ее в деструкторе поста.*/

void Cgi::parse_cgi_response()
{
	std::ifstream in("./.tmp_cgi");
	std::string all;
	int hdr_end;
	int hdr_start;

	getline(in, all, '\0');

	hdr_end = all.find("\r\n\r\n", 0);
	hdr_start = all.find("\r\n") + 2;
	_body = all.substr(hdr_end + 4);
	_content_length = _body.length();

	for (int i = 0; i < all.length(); ++i)
	{
		if (isdigit(all[i]))
		{
			_status_code = atoi(all.substr(i, 3).c_str());
			break;
		}
	}
	_headers = all.substr(hdr_start, hdr_end - hdr_start);
	_headers = _headers + "\r\nContent-Length: " + std::to_string(_content_length);
}

int Cgi::getStatusCode() const
{
	return _status_code;
}

const std::string &Cgi::getHeaders() const
{
	return _headers;
}

const std::string &Cgi::getBody() const
{
	return _body;
}

long long Cgi::getContentLength() const
{
	return _content_length;
}
