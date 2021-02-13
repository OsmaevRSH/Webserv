#include "Server.hpp"
#include "Parser.hpp"
#include "GetMimeTypeAndErrorPageCode.hpp"

int main(int argc, char **argv, char **env)
{
	MIME_ERROR mime;
#ifndef TESTER
	Parser config("./App/Config/conf");
#endif
#ifdef TESTER
	if (argc != 2)
	{
		std::cout << "Error: bad number of argument\n";
		exit(1);
	}
//	Parser config(argv[1]);
	Parser config("./App/Config/conf_for_tester");
#endif
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime,env);
	server.server_start();
	return 0;
}
