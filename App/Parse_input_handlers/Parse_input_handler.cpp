#include "Parse_input_handler.hpp"

const std::string &Parse_input_handler::getType() const
{
	return _type;
}

const std::string &Parse_input_handler::getUrl() const
{
	return _url;
}

Parse_input_handler::~Parse_input_handler()
{
}

Parse_input_handler &Parse_input_handler::operator=(const Parse_input_handler &copy)
{
	_protocol_type = copy._protocol_type;
	_type = copy._type;
	_url = copy._url;
	return *this;
}

Parse_input_handler::Parse_input_handler(const char *input, std::string &server_ip, std::string &client_ip)
		: _server_ip(server_ip), _client_ip(client_ip), _error(false)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string> handlers;
	std::string tmp = input;
	size_t iter;
	while ((iter = tmp.find("\r\n")) != std::string::npos && tmp[iter + 2] != '\0')
	{
		handlers.push_back(tmp.substr(0, iter));
		tmp.erase(0, iter + 2);
	}
	it = handlers.begin();
	tmp = *it++;
	parse_first_handler_string(tmp, _type);
	parse_first_handler_string(tmp, _url);
	parse_first_handler_string(tmp, _protocol_type);
	for (; it < handlers.end(); ++it)
	{
		iter = (*it).find(':');
		if (iter != std::string::npos)
		{
			if (_variable_handlers.find((*it).substr(0, iter)) == _variable_handlers.end())
				_variable_handlers[(*it).substr(0, iter)] = (*it).substr(iter + 2);
			else
			{
				_error = true;
				break;
			}
		}
	}
	headersIsOk();
}

void Parse_input_handler::parse_first_handler_string(std::string &tmp, std::string &handler)
{
	size_t iter;

	iter = tmp.find(' ');
	handler = tmp.substr(0, iter);
	tmp.erase(0, iter + 1);
}

void Parse_input_handler::setUrl(const std::string &url)
{
	_url = url;
}

const std::map<std::string, std::string> &Parse_input_handler::getVariableHandlers() const
{
	return _variable_handlers;
}

const std::string &Parse_input_handler::getServerIp() const
{
	return _server_ip;
}

const std::string &Parse_input_handler::getClientIp() const
{
	return _client_ip;
}

void Parse_input_handler::headersIsOk()
{
	std::map<std::string, std::string>::const_iterator cont_len = _variable_handlers.find("Content-Length");
	std::map<std::string, std::string>::const_iterator tr_enc = _variable_handlers.find("Transfer-Encoding");
	std::map<std::string, std::string>::const_iterator host = _variable_handlers.find("Host");
	std::map<std::string, std::string>::const_iterator ite = _variable_handlers.end();

	if (host == ite || (cont_len != ite && tr_enc != ite) || (_type != "GET" && _type != "HEAD" && _type != "PUT" && _type != "POST") ||
		_protocol_type != "HTTP/1.1")
		_error = true;
}

bool Parse_input_handler::isError() const
{
	return _error;
}
