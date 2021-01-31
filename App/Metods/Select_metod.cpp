#include "Parse_input_handler.hpp"
#include "Server.hpp"
#include "GET.hpp"
#include "HEAD.hpp"
#include "PUT.hpp"
#include "POST.hpp"

void Server::Method_selector(const Parse_input_handler &inputHandlers, std::string &handler, std::string &body, std::string &handler_body)
{
	if (inputHandlers.getType() == "GET")
	{
		GET get(_config, inputHandlers, _mime, handler, body);
		get.start_processing();
	}
	if (inputHandlers.getType() == "HEAD")
	{
		HEAD head(_config, inputHandlers, _mime, handler, body);
		head.start_processing();
	}
	if (inputHandlers.getType() == "PUT")
	{
		PUT put(_config, inputHandlers, _mime, handler, handler_body, body);
		put.start_processing();
	}
	if (inputHandlers.getType() == "POST")
	{
		POST post(_config, inputHandlers, _mime, handler, body, handler_body, env);
		post.start_processing();
		handler = "HTTP/1.1 405 METHOD NOT ALLOWED\r\n"
				  "Content-Type: text/plain\r\n"
				  "Content-Length: 0\r\n"
				  "Server: Webserver/1.0\r\n"
				  "Allow: GET\r\n\r\n";
	}
}