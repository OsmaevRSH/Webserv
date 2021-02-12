#include "POST.hpp"

POST::POST(const Serv_conf &serv, std::list<Client>::iterator &Iter, const MIME_ERROR &mime, std::string &head, std::string &body, std::string &handler_body, char **env)
		:	Search_by_configuration(serv, *Iter->_client_handler, mime, Iter),
			_body(body),
			_head(head),
			_handler_body(handler_body),
			_env(env),
			_cgi(nullptr){}

POST::~POST()
{
	delete _cgi;
	unlink(".tmp_cgi");
}

void POST::start_processing()
{
	Search_path();
	if (check_cgi_extension(_handler.getUrl(), _output.location.cgi_extension) && _output.status_code != 405)
	{
		init_cgi_struct();
		_cgi = new Cgi(_output.location.cgi_path, _cgi_struct);
		_cgi->parse_cgi_response();
		get_header_if_not_error();
	}
	else
	{
		get_header_if_error();
		_body = _mime.get_error_page(_output.status_code);
		return;
	}
}

std::string POST::get_content_length()
{
	std::string tmp;

	tmp = "Content-Length: " + std::to_string(_mime.get_error_page(_output.status_code).size()) + "\r\n";
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

		output << "HTTP/1.1 " << _cgi->getStatusCode() << " " << _mime.get_error(_cgi->getStatusCode()) << "\r\n";
		output << get_server_name();
		output << _cgi->getHeaders() << "\r\n";
		output << get_date_handler();
		output << "\r\n";
		_head = output.str();
		_body = _cgi->getBody();
}

bool POST::check_cgi_extension(const std::string &url, const std::string &cgiFileExtension)
{
	std::string tmp = url.substr(url.find_last_of('.') == std::string::npos ? 0 : url.find_last_of('.'));
	if (!strcmp(tmp.c_str(), cgiFileExtension.c_str()))
		return true;
	return false;
}

void POST::init_cgi_struct()
{
	_cgi_struct.pathToCgiScript = _output.path_to_file.empty() ? nullptr : const_cast<char *>(_output.path_to_file.c_str());
	_cgi_struct.body = _handler_body;
	_cgi_struct.headers = &_handler;
	_cgi_struct.port = _output.port;
	_cgi_struct.client_ip = _handler.getClientIp();
	_cgi_struct.path_info = "/";
	_cgi_struct.script_name = _handler.getUrl();
	_cgi_struct.server_ip = _handler.getServerIp();
	_cgi_struct.env = _env;
}
