#include "Parse_input_handler.hpp"
#include "Server.hpp"
#include "GET.hpp"
#include "HEAD.hpp"
#include "PUT.hpp"
#include "POST.hpp"

void Server::Method_selector(std::string &handler, std::string &body, std::list<Client>::iterator &Iter)
{
	if (Iter->_client_handler->getType() == "GET")
	{
		GET get(_config, *Iter->_client_handler, _mime, handler, body);
		get.start_processing();
	}
	if (Iter->_client_handler->getType() == "HEAD")
	{
		HEAD head(_config, *Iter->_client_handler, _mime, handler, body);
		head.start_processing();
	}
	if (Iter->_client_handler->getType() == "PUT")
	{
		PUT put(_config, *Iter->_client_handler, _mime, handler, Iter->_request_body, body);
		put.start_processing();
	}
	if (Iter->_client_handler->getType() == "POST")
	{
		POST post(_config, Iter, _mime, handler, body, Iter->_request_body, env);
		post.start_processing();
	}
}