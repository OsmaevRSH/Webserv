#include "POST.hpp"

POST::POST(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &head, std::string &body, std::string &handler_body, char **env)
		: Search_by_configuration(serv, handler, mime), _body(body), _head(head), _handler_body(handler_body), _env(env) {}

void POST::start_processing()
{
	char *dir;
	Search_path();
	_cgi_struct.body = _handler_body;
	_cgi_struct.headers = &_handler;
	_cgi_struct.port = _output.port;
	_cgi_struct.client_ip = _handler.getClientIp();
//	_cgi_struct.path_info = "";
	_cgi_struct.path_info = "./Tester/YoupiBanane/yaupi.bla";
	_cgi_struct.path_translated = getcwd(nullptr, 0) + _output.path_to_file.substr(1);
	_cgi_struct.script_name = _handler.getUrl();
	_cgi_struct.server_ip = _handler.getServerIp();
	_cgi_struct.env = _env;
	Cgi cgi("./Tester/cgi_tester", _cgi_struct);
	while(cgi.getResponse()){}
}

std::string POST::get_content_length()
{
	return std::string();
}
