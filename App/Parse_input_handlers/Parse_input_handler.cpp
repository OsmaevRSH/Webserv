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


Parse_input_handler::Parse_input_handler(const char *input)
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
	parse_first_handler_string(tmp, _type);
	parse_first_handler_string(tmp, _url);
	parse_first_handler_string(tmp, _protocol_type);
	for (; it < handlers.end(); ++it)
	{
		iter = (*it).find(':');
		if (iter != std::string::npos)
			_variable_handlers[(*it).substr(0, iter)] = (*it).substr(iter + 2);
	}
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

void Parse_input_handler::setBody(const std::string &body)
{
	_body = body;
}

const std::string &Parse_input_handler::getBody() const
{
	return _body;
}
