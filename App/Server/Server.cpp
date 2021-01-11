#include "Server.hpp"

_Noreturn void Server::ListenLoop()
{
	int max_fd = 0;
	std::vector<int>::iterator Iter;

	while (true)
	{
		Reset_fd_set();
		Add_new_fd_to_set();
		Search_max_fd(max_fd);
		int res = select(max_fd + 1, &_readfds, &_writefds, nullptr, nullptr);
		Checkout_call_to_select(res);
		Accept_if_serv_fd_changed();
		Iter = _read_socket_fd.begin();
		while (Iter != _read_socket_fd.end())
		{
			if (FD_ISSET(*Iter, &_readfds))
				Act_if_readfd_changed(Iter);
			else
				++Iter;
		}
		Iter = _write_socket_fd.begin();
		while (Iter != _write_socket_fd.end())
		{
			if (FD_ISSET(*Iter, &_writefds))
				Act_if_writefd_changed(Iter);
			else
				++Iter;
		}
	}
}

void Server::Act_if_writefd_changed(std::vector<int>::iterator &Iter)
{
	//std::cout << "WRITE_SET!!!\n";
	//	int send_number;
	/*send_number = */send(*Iter, (_request_to_client[*Iter][0] +
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
	//		shutdown(*Iter, SHUT_RDWR);
	//		close(*Iter);
	_request_to_client.erase(*Iter);
	_read_socket_fd.push_back(*Iter);
	Iter = _write_socket_fd.erase(Iter);
	//	}

}

bool Server::Checkout_call_to_select(const int &res)
{
	if (res < 1)
	{
		if (errno != EINTR)
		{
			perror("Select error");
			exit(1);
		}
		else
			exit(0);
	}
	if (res == 0)
		return true;
	return false;
}

void Server::Act_if_readfd_changed(std::vector<int>::iterator &Iter)
{
	char *buffer;
	int recv_res;
	char *buf = (char *) malloc(sizeof(char) * 576);
	recv_res = recv(*Iter, buf, 575, 0);
	if (recv_res > 0)
		buf[recv_res] = '\0';
	if (recv_res == -1 || (buffer = check_input_handler_buffer(buf, Iter)) == nullptr)
	{
		++Iter;
		return;
	}
	Input_handlers inputHandlers(buffer);
#ifdef SERVER_DEBUG
	inputHandlers.output();
#endif
	if ((recv_res == 0 &&
		 _request_to_client.find(*Iter) == _request_to_client.end()) ||
		(inputHandlers.getVariableHandlers().find("Connection") !=
		 inputHandlers.getVariableHandlers().end() &&
		 inputHandlers.getVariableHandlers().at("Connection") == "close"))
	{
		close(*Iter);
		Iter = _read_socket_fd.erase(Iter);
	}
	std::string file = _config.Handler(inputHandlers.getHandlers(), inputHandlers);
	std::vector<std::string> tmp;
	tmp.push_back(
			"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-Length: " +
			std::to_string(file.size()) + "\r\n\r\n");
	tmp.push_back(file);
	_request_to_client.insert(std::pair<int, std::vector<std::string> >(*Iter, tmp));
	_write_socket_fd.push_back(*Iter);
	Iter = _read_socket_fd.erase(Iter);
}