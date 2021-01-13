#include "Parse_input_handler.hpp"
#include "Server.hpp"
#include "GET/GET.hpp"

void Server::Method_selector(const Parse_input_handler &inputHandlers, std::string &handler, std::string &body)
{
	if (inputHandlers.getType() == "GET")
	{
		GET get(_config, inputHandlers);
		body = get.get_page();
		handler = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " +
				  std::to_string(body.size()) + "\r\n\r\n";
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
		std::cerr << "Error: Invalid method\n";
		exit(EXIT_FAILURE);
	}
}