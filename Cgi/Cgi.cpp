#include "Cgi.h"

Cgi::Cgi(const std::string &path_to_cgi, const t_data_for_cgi &data)
{
	char **env = (char**)malloc(sizeof(char*) * 18);
	std::string value;
	std::string name;

	for (int i = 0; i < 18; i++)
		env[i] = NULL;

	env[0] = strdup("SERVER_SOFTWARE = Webserver");
	env[1] = strdup("GATEWAY_INTERFACE = CGI/1.1");
	env[2]

	value = data.headers.getVariableHandlers().at("Content-type");
	if (value.empty() == false)
	{
		name = "CONTENT_TYPE = " + value;
		env[0] = strdup(name.c_str());
	}

	if (data.body.empty() == false)
	{
		value = data.headers.getVariableHandlers().at("Content-length");
		if (value.empty() == false)
		{
			name = "CONTENT_LENGTH = " + value;
			env[1] = strdup(name.c_str());
		}
	}


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