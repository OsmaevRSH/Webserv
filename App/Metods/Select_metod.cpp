#include "Parse_request_headers.hpp"
#include "Server.hpp"
#include "GET.hpp"
#include "HEAD.hpp"
#include "PUT.hpp"
#include "POST.hpp"

void Server::Method_selector(std::string &handler, std::string &body, std::list<Client>::iterator &Iter)
{
	if (Iter->_client_handler->getType() == "GET")
	{
		GET get(_config, *Iter->_client_handler, _mime, handler, body, Iter);
		get.start_processing();
	}
	else if (Iter->_client_handler->getType() == "HEAD")
	{
		HEAD head(_config, *Iter->_client_handler, _mime, handler, body, Iter);
		head.start_processing();
	}
	else if (Iter->_client_handler->getType() == "PUT")
	{
		PUT put(_config, *Iter->_client_handler, _mime, handler, Iter->_request_body, body, Iter);
		put.start_processing();
	}
	else if (Iter->_client_handler->getType() == "POST")
	{
		POST post(_config, Iter, _mime, handler, body, Iter->_request_body, env);
		post.start_processing();
	}
	else
	{
		body = _config._error_pages.find(501) == _config._error_pages.end() ? _mime.get_error_page(501) : _config._error_pages[501];
		handler = "HTTP/1.1 501 NOT IMPLEMENTED\r\n"
		          "Content-Type: text/html\r\n"
		          "Content-Length: " + std::to_string(body.size()) + "\r\n"
		                                                             "Server: Webserver/1.0\r\n\r\n";
	}
}