#include "Parce_input_handler.hpp"

const std::string &Parce_input_handler::getType() const
{
	return _type;
}

const std::string &Parce_input_handler::getUrl() const
{
	return _url;
}

const std::string &Parce_input_handler::getProtocolType() const
{
	return _protocol_type;
}

Parce_input_handler::~Parce_input_handler()
{
}

Parce_input_handler &Parce_input_handler::operator=(const Parce_input_handler &copy)
{
	_protocol_type = copy._protocol_type;
	_type = copy._type;
	_url = copy._url;
	_handlers = copy._handlers;
	return *this;
}

Parce_input_handler::Parce_input_handler(const Parce_input_handler &copy)
		: _url(copy._url), _type(copy._type),
		_protocol_type(copy._protocol_type), _handlers(copy._handlers) {}

Parce_input_handler::Parce_input_handler(const char *input)
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
	std::map<std::string, std::string &>::iterator it1;
	std::map<std::string, std::string>::iterator save_it1;
	for (it1 = _config_list.begin(); it1 != _config_list.end(); ++it1)
	{
		if ((save_it1 = _variable_handlers.find(it1->first)) != _variable_handlers.end())
		{
			it1->second = save_it1->second;
#ifdef HANDLERS_DEBUG
			std::cout << save_it1->first << ": " << save_it1->second << std::endl;
#endif
		}
	}
}

void Parce_input_handler::parce_first_handler_string(std::string &tmp, std::string &handler)
{
	size_t iter;

	iter = tmp.find(' ');
	handler = tmp.substr(0, iter);
	tmp.erase(0, iter + 1);
}

void Parce_input_handler::Map_init()
{
	_config_list.insert(std::pair<std::string, std::string &>("Accept-Charsets", _handlers.Accept_Charsets));
	_config_list.insert(std::pair<std::string, std::string &>("Accept-Language", _handlers.Accept_Language));
	_config_list.insert(std::pair<std::string, std::string &>("Allow", _handlers.Allow));
	_config_list.insert(std::pair<std::string, std::string &>("Authorization", _handlers.Authorization));
	_config_list.insert(std::pair<std::string, std::string &>("Content-Language", _handlers.Content_Language));
	_config_list.insert(std::pair<std::string, std::string &>("Content-Length", _handlers.Content_Length));
	_config_list.insert(std::pair<std::string, std::string &>("Content-Location", _handlers.Content_Location));
	_config_list.insert(std::pair<std::string, std::string &>("Content-Types", _handlers.Content_Type));
	_config_list.insert(std::pair<std::string, std::string &>("Date", _handlers.Date));
	_config_list.insert(std::pair<std::string, std::string &>("Host", _handlers.Host));
	_config_list.insert(std::pair<std::string, std::string &>("Last-Modified", _handlers.Last_Modified));
	_config_list.insert(std::pair<std::string, std::string &>("Location", _handlers.Location));
	_config_list.insert(std::pair<std::string, std::string &>("Referer", _handlers.Referer));
	_config_list.insert(std::pair<std::string, std::string &>("Retry-After", _handlers.Retry_After));
	_config_list.insert(std::pair<std::string, std::string &>("Server", _handlers.Server));
	_config_list.insert(std::pair<std::string, std::string &>("Transfer-Encoding", _handlers.Transfer_Encoding));
	_config_list.insert(std::pair<std::string, std::string &>("User-Agent", _handlers.User_Agent));
	_config_list.insert(std::pair<std::string, std::string &>("WWW-Authenticate", _handlers.WWW_Authenticate));
}

t_headers &Parce_input_handler::getHandlers()
{
	return _handlers;
}

void Parce_input_handler::setUrl(const std::string &url)
{
	_url = url;
}

const std::map<std::string, std::string> &Parce_input_handler::getVariableHandlers() const
{
	return _variable_handlers;
}
