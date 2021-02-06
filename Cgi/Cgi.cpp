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

Cgi::Cgi(const std::string &path_to_cgi, const t_data_for_cgi &data) : _path_to_cgi(path_to_cgi), \
																		_data(data), \
																		_env(get_meta_variables(data)), \
																		_is_end(false)
{
	_args[0] = const_cast<char *>(_path_to_cgi.c_str());
	_args[1] = strdup("./Tester/YoupiBanane/youpi.bla");
	_args[2] = NULL;
	handleRequest();
}

Cgi::~Cgi()
{
	free(_buf);
	if (!_is_end)
		close(_pipe[0]);
}

void Cgi::handleRequest()
{
	int fd[2], status, child;

	status = pipe(fd);
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
		waitpid(child, NULL, 0);
/*
		tmp_fd = open("./.tmp_cgi", O_RDONLY);
		_buf = (char*)calloc(100000000, 1);
		read(tmp_fd, _buf, 100000000);
		write(1, _buf, 100000000);*/
	}
}

const char *Cgi::getResponse()
{
	int res;

	if (_is_end == true)
		return NULL;
	bzero(_buf, 100000001);
	res = read(_pipe[0], _buf, 100000000);
	write(1, _buf, res);
	if (res == 0)
	{
		_is_end = true;
		close(_pipe[0]);
	}
	return _buf;
}

/* Тут нужно определиться с возвращаемым типом. Если тупо копировать строку, то это может быть долго.
 * Но тогда нужно выделять память под сишную строку, и потом не забыть ее отчистить.
 * Хм... Отдам-ка я сишную строку и отчистю ее в деструкторе поста.*/

char *Cgi::parse_cgi_response() {
	std::ifstream in("./.tmp_cgi");
	std::string all;
	std::string body;
	std::string headers;
	long long 	len;
	int 		hdr_end;

	getline(in, all, '\0');
	hdr_end = all.find("\r\n\r\n", 0);
	headers = all.substr(0, hdr_end);
	body = all.substr(hdr_end);
	len = body.length();
	headers = headers + "\r\nContent-Length: " + std::to_string(len);
	all = headers + body;

	return (strdup(all.c_str()));
}