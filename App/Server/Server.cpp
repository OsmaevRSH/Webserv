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
	Input_handlers *inputHandlers;
	std::string handler;
	std::string body;

	if ((inputHandlers = Reading_a_request(Iter)) == nullptr)
		return;
#ifdef SERVER_DEBUG
	inputHandlers.output();
#endif
//	Method_selector(*inputHandlers, handler, body);
	std::string file = _config.Handler(inputHandlers->getHandlers(), *inputHandlers);
//	tmp.push_back(handler);
//	tmp.push_back(body);
	tmp.push_back(
			"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " +
			std::to_string(file.size()) + "\r\n\r\n");
	tmp.push_back(file);
	_request_to_client.insert(std::pair<int, std::vector<std::string> >(*Iter, tmp));
	_write_socket_fd.push_back(*Iter);
	Iter = _read_socket_fd.erase(Iter);
}