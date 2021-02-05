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
		: _path_to_cgi(path_to_cgi), _data(data), _env(get_meta_variables(data)), _is_end(false)
{
	_args[0] = const_cast<char *>(_path_to_cgi.c_str());
	_args[1] = strdup("./Tester/YoupiBanane/youpi.bla");
	_args[2] = NULL;
//	_data.body = "nnnnnnnnnnnnnnnnnnnnn";
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
	tmp_fd = open("./test", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
//	if (status < 0 || tmp_fd < 0 || (child = fork ()) < 0)
//		throw (std::runtime_error(strerror(errno)));
	child = fork();
	if (child == 0)
	{
		close(fd[1]); //ничего не пишем
		//заменяем stdout дочернего процесса на дескриптор временного файла
		dup2(tmp_fd, 1);
		//читать запрос будем от родителя
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
//		if (WIFEXITED(status)) {
//			int exit_code = WEXITSTATUS(status);
//			if (exit_code == 2)
//				return;
//		}
		tmp_fd = open("./test", O_RDONLY);
		_buf = (char*)calloc(1025, 1);
		read(tmp_fd, _buf, 1024);
		write(1, _buf, 1024);
	}
}

const char *Cgi::getResponse()
{
	int res;
	/*for (int i = 0; i < 100; ++i)
	{
		tmp = *(_env + i);
	}*/

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