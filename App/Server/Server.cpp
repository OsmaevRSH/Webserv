#include "Server.hpp"

_Noreturn void Server::ListenLoop()
{
	int max_fd = 0;
	int res;

	while (true)
	{
		Reset_fd_set();
		Add_new_fd_to_set();
		Search_max_fd(max_fd);
		res = select(max_fd + 1, &_readfds, &_writefds, nullptr, nullptr);
		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed();
		Check_read_set();
		Check_write_set();
	}
}

void Server::Act_if_writefd_changed(std::vector<int>::iterator &Iter)
{
//	int send_number;
//	send_number =
	send(*Iter, (_request_to_client[*Iter][0] +
				 _request_to_client[*Iter][1]).c_str(),
			_request_to_client[*Iter][0].size() +
			_request_to_client[*Iter][1].size(), 0);
//	if (send_number < static_cast<int>(_request_to_client[*Iter][0].size() +
//									   _request_to_client[*Iter][1].size()))
//	{
//		if (send_number <=
//			static_cast<int>(_request_to_client[*Iter][0].size()))
//			_request_to_client[*Iter][0].erase(0, send_number);
//		else
//		{
//			_request_to_client[*Iter][1].erase(0,
//					send_number - _request_to_client[*Iter][0].size());
//			_request_to_client[*Iter][0].erase(_request_to_client[*Iter][0].begin(), _request_to_client[*Iter][0].end());
//		}
//	}
//	else
//	{
//		close(*Iter);
	_request_to_client.erase(*Iter);
	_read_socket_fd.push_back(*Iter);
	Iter = _write_socket_fd.erase(Iter);
//	}
}

void Server::Act_if_readfd_changed(std::vector<int>::iterator &Iter)
{
	std::vector<std::string> tmp;
	Parse_input_handler *inputHandlers;
	std::string handler;
	std::string body;

	if ((inputHandlers = Reading_a_request(Iter)) == nullptr)
		return;
#ifdef SERVER_DEBUG
	inputHandlers.output();
#endif
	Method_selector(*inputHandlers, handler, body);
	tmp.push_back(handler);
	tmp.push_back(body);
	_request_to_client.insert(std::pair<int, std::vector<std::string> >(*Iter, tmp));
	_write_socket_fd.push_back(*Iter);
	Iter = _read_socket_fd.erase(Iter);
}

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
