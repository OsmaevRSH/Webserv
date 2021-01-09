#include "Server.hpp"

std::string Server::check_input_handler_buffer(const char *input_buffer, std::vector<int>::iterator &Iter)
{
	std::string check_buffer = input_buffer;
	if (check_buffer.find("\r\n\r\n"))
	{
		if (_input_handler_buffer.find(*Iter) == _input_handler_buffer.end())
			return check_buffer;
		return _input_handler_buffer[*Iter] + check_buffer;
	}
	else
	{
		if (_input_handler_buffer.find(*Iter) == _input_handler_buffer.end())
			_input_handler_buffer.insert(std::pair<int, std::string>(*Iter, check_buffer));
		else
			_input_handler_buffer[*Iter] += check_buffer;
	}
	return reinterpret_cast<const char *>(std::string::npos);
}

