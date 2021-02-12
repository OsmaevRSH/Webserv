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
//	Parser config(argv[1]);
//	Parser config("./App/Config/conf_for_tester");
	Parser config("./App/Config/conf_wordpress");
#endif
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime,env);
	server.server_start();
	return 0;
}
