#include "Server.hpp"
#include "Config.hpp"

int main()
{
	Config config("./Test/conf");
	Server test(config.getServers());
	test.server_start();
	return 0;
}
