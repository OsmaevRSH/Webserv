#include "../Server/Server.hpp"

char *Server::check_input_handler_buffer(char *input_buffer, std::vector<int>::iterator &Iter)
{
	char *tmp_return_buffer;
	std::string check_buffer = input_buffer;
	if (check_buffer.find("\r\n\r\n") != std::string::npos)
	{
		if (_input_handler_buffer.find(*Iter) == _input_handler_buffer.end())
			return strdup(check_buffer.c_str());
		tmp_return_buffer = strdup((_input_handler_buffer[*Iter] + check_buffer).c_str());
		_input_handler_buffer.erase(*Iter);
		return tmp_return_buffer;
	}
	else
	{
		if (_input_handler_buffer.find(*Iter) == _input_handler_buffer.end())
			_input_handler_buffer.insert(std::pair<int, std::string>(*Iter, check_buffer));
		else
			_input_handler_buffer[*Iter] += check_buffer;
	}
	return nullptr;
}

