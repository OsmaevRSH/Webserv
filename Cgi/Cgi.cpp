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
}

static void send_body_to_cgi(const std::string &body, int *pipe_fd)
{
	write(1, body.c_str(), strlen(body.c_str()));
}

static **char create_env()
{

}

std::string &Cgi::handleRequest() {
	int		pipe_fd[2];
	int 	save_stdout;
	int 	save_stdin;
	pid_t	pid;


	pipe(pipe_fd);
	save_stdout = dup(1);
	save_stdin = dup(0);
	dup2(pipe_fd[1], 1);
	send_body_to_cgi(/* body */);
	close(pipe_fd[1]);

	pid = fork();
	if (pid == 0)
	{
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execve(_path_to_sgi, _args, _env);
	}
	else
	{
		waitpid(pid, NULL, 0);
		dup2(save_stdout, 0);
		dup2(save_stdin, 1);
	}
}