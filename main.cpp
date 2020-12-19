#include "Server.hpp"

int main()
{
	std::vector<std::map<std::string, std::string> > servers;
	std::map<std::string, std::string> serv1;
	serv1.insert(std::make_pair("address", "127.0.0.1"));
	serv1.insert(std::make_pair("port", "8000"));
	servers.push_back(serv1);
	std::map<std::string, std::string> serv2;
	serv2.insert(std::make_pair("address", "127.0.0.1"));
	serv2.insert(std::make_pair("port", "8008"));
	servers.push_back(serv2);

	Server test(servers);
	test.server_start();
	return 0;
}
