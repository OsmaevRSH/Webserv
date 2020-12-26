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
	Map_init();
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
	std::map<std::string, std::string>::iterator it1;
	for (it1 = _variable_handlers.begin(); it1 != _variable_handlers.end(); ++it1)
	{

	}
}

void Input_handlers::parce_first_handler_string(std::string &tmp, std::string &handler)
{
	size_t iter;

	iter = tmp.find(' ');
	handler = tmp.substr(0, iter);
	tmp.erase(0, iter + 1);
}

void Input_handlers::Map_init()
{
	_config_list["Accept-Charsets"] = _handlers.Accept_Charsets;
	_config_list["Accept-Language"] = _handlers.Accept_Language;
	_config_list["Allow"] = _handlers.Allow;
	_config_list["Authorization"] = _handlers.Authorization;
	_config_list["Content-Language"] = _handlers.Content_Language;
	_config_list["Content-Length"] = _handlers.Content_Length;
	_config_list["Content-Location"] = _handlers.Content_Location;
	_config_list["Content-Type"] = _handlers.Content_Type;
	_config_list["Date"] = _handlers.Date;
	_config_list["Host"] = _handlers.Host;
	_config_list["Last-Modified"] = _handlers.Last_Modified;
	_config_list["Location"] = _handlers.Location;
	_config_list["Referer"] = _handlers.Referer;
	_config_list["Retry-After"] = _handlers.Retry_After;
	_config_list["Server"] = _handlers.Server;
	_config_list["Transfer-Encoding"] = _handlers.Transfer_Encoding;
	_config_list["User-Agent"] = _handlers.User_Agent;
	_config_list["WWW-Authenticate"] = _handlers.WWW_Authenticate;
}
