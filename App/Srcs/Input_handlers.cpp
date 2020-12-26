#include "Input_handlers.hpp"

const std::string &Input_handlers::getType() const
{
	return _type;
}

const std::string &Input_handlers::getUrl() const
{
	return _url;
}

const std::string &Input_handlers::getProtocolType() const
{
	return _protocol_type;
}

Input_handlers::~Input_handlers()
{
}

Input_handlers &Input_handlers::operator=(const Input_handlers &copy)
{
	_protocol_type = copy._protocol_type;
	_type = copy._type;
	_url = copy._url;
	_handlers = copy._handlers;
	return *this;
}

Input_handlers::Input_handlers(const Input_handlers &copy)
		: _url(copy._url), _type(copy._type),
		_protocol_type(copy._protocol_type), _handlers(copy._handlers) {}

Input_handlers::Input_handlers(const char *input)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string> handlers;
	std::string tmp = input;
	size_t iter;
	while ((iter = tmp.find("\r\n")) != std::string::npos &&
		   tmp[iter + 2] != '\0')
	{
		handlers.push_back(tmp.substr(0, iter));
		tmp.erase(0, iter + 2);
	}
	it = handlers.begin();
	tmp = *it++;
	parce_first_handler_string(tmp, _type);
	parce_first_handler_string(tmp, _url);
	parce_first_handler_string(tmp, _protocol_type);
	for (; it < handlers.end(); ++it)
	{
		iter = (*it).find(':');
		if (iter != std::string::npos)
			_variable_handlers[(*it).substr(0, iter)] = (*it).substr(iter + 2);
	}
}

void Input_handlers::parce_first_handler_string(std::string &tmp, std::string &handler)
{
	size_t iter;

	iter = tmp.find(' ');
	handler = tmp.substr(0, iter);
	tmp.erase(0, iter + 1);
}
