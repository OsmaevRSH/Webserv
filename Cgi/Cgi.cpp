#include "Cgi.h"

std::string &Cgi::handleRequest() {
	int		pipe_fd[2];
	int 	save_stdout;
	int 	save_stdin;
	pid_t	pid;

	pipe(pipe_fd);
	save_stdout = dup(1);
	save_stdin = dup(0);
	dup2(pipe_fd[1], 1);
	// ТУТ БУДЕТ ФУНКЦИЯ, которая пишет тело сообщения в strout
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