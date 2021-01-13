#include "Parse_input_handler.hpp"
#include "Server.hpp"
#include "GET.hpp"

void Server::Method_selector(const Parse_input_handler &inputHandlers, std::string &handler, std::string &body)
{
	if (inputHandlers.getType() == "GET")
	{
		GET get(_config, inputHandlers, _mime);
		body = get.get_page();
		handler = get.get_hendler(body);
	}
	else if (inputHandlers.getType() == "HEAD")
	{}
	else if (inputHandlers.getType() == "PUT")
	{}
	else if (inputHandlers.getType() == "POST")
	{}
	else if (inputHandlers.getType() == "TRACE")
	{}
	else if (inputHandlers.getType() == "DELETE")
	{}
	else if (inputHandlers.getType() == "CONNECT")
	{}
	else if (inputHandlers.getType() == "OPTIONS")
	{}
	else
	{
		std::cerr << "|" << inputHandlers.getType() << "|" << " Error: Invalid method\n";
		exit(EXIT_FAILURE);
	}
}