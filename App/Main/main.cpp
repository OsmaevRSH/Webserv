#include "Server.hpp"
#include "Parser.hpp"
#include "GetMimeTypeAndErrorPageCode.hpp"

int main(int argc, char **argv, char **env)
{
	if (argc != 2)
	{
		std::cout << "Error: bad number of argument\n";
		exit(1);
	}
	MIME_ERROR mime;
	Parser config(argv[1]);
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime,env);
	server.server_start();
	return 0;
}
