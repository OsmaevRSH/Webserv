#include "POST.hpp"

POST::POST(const Serv_conf &serv, \
		   std::list<Client>::iterator &Iter, \
		   const MIME_ERROR &mime, \
		   std::string &head, \
		   std::string &body, \
		   std::string &handler_body, \
		   char **env) : \
		   Search_by_configuration(serv, *Iter->_client_handler, mime), \
		   _body(body), \
		   _head(head), \
		   _handler_body(handler_body), \
		   _env(env), \
		   _iter(Iter), \
		   _response_for_cgi(NULL) {}

POST::~POST()
{
	free(_response_for_cgi);
}

void POST::start_processing()
{
	if (_iter->_curent_progress == 1)
	{
		Search_path();
		if (_output.status_code == 200)
		{
			_cgi_struct.body = _handler_body;
			_cgi_struct.headers = &_handler;
			_cgi_struct.port = _output.port;
			_cgi_struct.client_ip = _handler.getClientIp();
			_cgi_struct.path_info = "/";
			_cgi_struct.script_name = _handler.getUrl();
			_cgi_struct.server_ip = _handler.getServerIp();
			_cgi_struct.env = _env;
			_cgi = new Cgi("./Tester/cgi_tester", _cgi_struct);
		}
		else
		{
			get_header_if_error();
			_body = _config._error_pages[_output.status_code];
			return;
		}
		++_iter->_curent_progress;
//		get_header_if_not_error();
	}
	std::cerr << "Read" << std::endl;
	_cgi->parse_cgi_response();
}

std::string POST::get_content_length()
{
	std::string tmp;

	tmp = "Content-Length: " + std::to_string(_config._error_pages[_output.status_code].size()) + "\r\n";
	return tmp;
}

void POST::get_header_if_error()
{
	std::stringstream output;

	output << "HTTP/1.1 " << _output.status_code << " " << _mime.get_error(_output.status_code) << "\r\n";
	output << get_server_name();
	output << get_content_length();
	output << get_date_handler();
	output << get_content_type();
	if (_output.status_code == 405)
		output << get_allow_methods();
	output << "\r\n";
	_head = output.str();
}

void POST::get_header_if_not_error()
{
	std::stringstream output;

	output << "HTTP/1.1 " << 200 << " " << "OK" << "\r\n";
	output << get_server_name();
	output << "Content-Length: 100000000\r\n";
	output << get_date_handler();
	output << "Content-Type: text/html\r\n";
	output << "\r\n";
	_head = output.str();
	char *test = (char *)calloc(100000001, 1);
	memset(test, 'E', 100000000);
	_body = test;
}