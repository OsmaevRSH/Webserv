#include "Parse_input_handler.hpp"
#include "Server.hpp"
#include "GET.hpp"
#include "HEAD.hpp"
#include "PUT.hpp"

void Server::Method_selector(const Parse_input_handler &inputHandlers, std::string &handler, std::string &body)
{
	if (inputHandlers.getType() == "GET")
	{
		GET get(_config, inputHandlers, _mime, handler, body);
		get.start_processing();
	}
	else if (inputHandlers.getType() == "HEAD")
	{
		HEAD head(_config, inputHandlers, _mime, handler, body);
		head.start_processing();
	}
//	else if (inputHandlers.getType() == "PUT")
//	{
//		PUT put(_config, inputHandlers, _mime, handler);
//		put.start_processing();
//	}
//	else if (inputHandlers.getType() == "POST")
//	{}
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