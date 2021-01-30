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
	_env = get_meta_variables(data);
	handleRequest();

}

static void send_body_to_cgi(const std::string &body, int *pipe_fd)
{
	write(1, body.c_str(), strlen(body.c_str()));
}
const std::string	&Cgi::getResponse() const
{
	return (_response);
}

const std::string &Cgi::handleRequest() {
	int		pipe_send[2];
	int		pipe_recv[2];
	int 	save_stdout;
	int 	save_stdin;
	pid_t	pid;

	pipe(pipe_send);
	pipe(pipe_recv);

	save_stdout = dup(1);
	save_stdin = dup(0);

	dup2(pipe_send[1], 1);
	close(pipe_send[1]);
	send_body_to_cgi(_data.body);

	pid = fork();
	if (pid == 0)
	{
		dup2(pipe_send[0], 0);
		close(pipe_send[0]);

		dup2(pipe_recv[1], 1);
		close(pipe_recv[1]);

		execve(_path_to_sgi, _args, _env);
	}
	else
	{
		close(pipe_send[0]);
		close(pipe_recv[1]);

		waitpid(pid, NULL, 0);

		dup2(pipe_recv[0], 0);
		close(pipe_recv[0]);

		char *buf = calloc(1024, 1);
		while (read(1, buf, 1024))
		{
			// обрабатываем и зануляем;
			bzero(buf, 1024);
		}
		free(buf);

		dup2(save_stdout, 0);
		dup2(save_stdin, 1);
	}
	return
}

/*
 *  Общая логикая: есть два пайпа, одним мы посылаем данные в cgi, другой получаем.
 *
 * */