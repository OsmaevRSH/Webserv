#include "POST.hpp"

POST::POST(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &head, std::string &body, std::string &handler_body)
		: Search_by_configuration(serv, handler, mime), _body(body), _head(head), _handler_body(handler_body) {}

void POST::start_processing()
{
	char *dir;
	Search_path();
	_cgi_struct.body = _handler_body;
	_cgi_struct.headers = _handler;
	_cgi_struct.port = _output.port;
//	_cgi_struct.client_ip = _handler.getIp();
	_cgi_struct.path_info = _output.path_to_file;
	_cgi_struct.path_translated = getcwd(nullptr, 0) + _output.path_to_file;
	_cgi_struct.script_name = _handler.getUrl();
	_cgi_struct.server_ip = _handler.getIp();
	Cgi cgi("./Tester/cgi_tester", _cgi_struct);
}

std::string POST::get_content_length()
{
	return std::string();
}
