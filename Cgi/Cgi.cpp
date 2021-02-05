#include "Cgi.h"

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

Cgi::Cgi(const std::string &path_to_cgi, const t_data_for_cgi &data)
{
	_path_to_cgi = path_to_cgi;
	_data = data;
	_args[0] = const_cast<char *>(_path_to_cgi.c_str());
	_args[1] = strdup("/Users/ltheresi/CLionProjects/Webserv/Tester/YoupiBanane/youpi.bla");
	_args[2] = NULL;
	_data.body = "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn";
	_env = get_meta_variables(data);
	_buf = (char *) calloc(1025, 1);
	_is_end = false;
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
	pid_t pid;
	_buf = (char *) calloc(1025, 1);

//	pipe(_pipe);
	pipe(_pipe_body);

	pid = fork();
	std::cout << _data.body.size() << std::endl;
	if (pid == 0)
	{
//		close(_pipe[0]);
//		dup2(_pipe[1], 1);
//		close(_pipe[1]);

		close(_pipe_body[1]);
		dup2(_pipe_body[0], 0);
		close(_pipe_body[0]);

		int fd = open("/Users/ltheresi/CLionProjects/Webserv/Html/test.txt", O_CREAT);
		if (execve(_args[0], _args, _env) == -1)
		{
			write(fd, "Error", 5);
			close(fd);
			exit(1);
		}
		close(fd);
		exit(0);
	}
	else
	{
		close(_pipe_body[0]);
		write(_pipe_body[1], _data.body.c_str(), _data.body.size());
		close(_pipe_body[1]);

//		close(_pipe[1]);

		waitpid(pid, NULL, 0);
	}
}

const char *Cgi::getResponse()
{
	int res;
	char *tmp = *_env;
	/*for (int i = 0; i < 100; ++i)
	{
		tmp = *(_env + i);
	}*/

	if (_is_end == true)
		return NULL;
	bzero(_buf, 1024);
	res = read(_pipe[0], _buf, 1024);
	//write(1, _buf, res);
	if (res == 0)
	{
		_is_end = true;
		close(_pipe[0]);
	}
	return _buf;
}

/*
 * Как работает этот кусок дерьма? Ха, да очень просто.
 * Создаем объект класса. Он сразу запускает cgi и все передает ему.
 * Формирует мета-переменные, всю хуйню и передает.
 * Далее мы просто вызываем функцию getResponse(), которая будет
 * получать от cgi данные по 1024 байт за раз и вот с ними мы будем уже работать.
 * Возвращается сишная строка. Ее нужно стрдапнуть, чистить ее не нужно! Как только
 * считаем все, фунция вернет NULL и вернет обратно подмененные fd. Если
 * вдруг считываение прерветься - не беда, в деструкторе все закроется и унчитожится,
 * главное, чтобы он был вызван.
 * */

/*int		pipe_send[2];
int		pipe_recv[2];

pipe(pipe_send);
pipe(pipe_recv);

_save_stdout = dup(1);
_save_stdin = dup(0);

dup2(pipe_send[1], 1);
close(pipe_send[1]);
send_body_to_cgi(_data.body);
*/
//close(fd_file);
/*
close(pipe_send[0]);
close(pipe_recv[1]);
waitpid(pid, NULL, 0);
dup2(pipe_recv[0], 0);
close(pipe_recv[0]);*/

/*dup2(pipe_send[0], 0);
		close(pipe_send[0]);
		dup2(pipe_recv[1], 1);
		close(pipe_recv[1]);*/