#include "Parse_input_handler.hpp"
#include "Server.hpp"
#include "GET.hpp"
#include "HEAD.hpp"
#include "PUT.hpp"

void Server::Method_selector(const Parse_input_handler &inputHandlers, std::string &handler, std::string &body, std::string &handler_body)
{
	if (inputHandlers.getType() == "GET")
	{
		std::cout << "GET!!!\n";
		GET get(_config, inputHandlers, _mime, handler, body);
		get.start_processing();
	}
	else if (inputHandlers.getType() == "HEAD")
	{
		HEAD head(_config, inputHandlers, _mime, handler, body);
		head.start_processing();
	}
	else if (inputHandlers.getType() == "PUT")
	{
		PUT put(_config, inputHandlers, _mime, handler, handler_body, body);
		put.start_processing();
	}
	else if (inputHandlers.getType() == "POST")
	{
		handler = "HTTP/1.1 405 METHOD NOT ALLOWED\r\n"
				  "Content-Type: text/plain\r\n"
				  "Content-Length: 0\r\n"
				  "Server: Webserver/1.0\r\n"
				  "Allow: GET\r\n\r\n";
	}
//	else if (inputHandlers.getType() == "TRACE")
//	{}
//	else if (inputHandlers.getType() == "DELETE")
//	{}
//	else if (inputHandlers.getType() == "CONNECT")
//	{}
//	else if (inputHandlers.getType() == "OPTIONS")
//	{}
	else
	{
		handler = "HTTP/1.1 405 METHOD NOT ALLOWED\r\n\r\n";
	}
}