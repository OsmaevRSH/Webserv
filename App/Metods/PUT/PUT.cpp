#include "PUT.hpp"

PUT::PUT(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &head, std::string &handler_body, std::string &body)
		: Search_by_configuration(serv, handler, mime), _head(head), _handler_body(handler_body), _body(body) {}

void PUT::get_PUT_status()
{
	this->Search_path();

	if (_output.status_code == 200 && _output.autoindex_page.empty())
		_status_code = 204;
	else if (_output.status_code == 404 || !_output.autoindex_page.empty())
		_status_code = 201;
	else
		_status_code = _output.status_code;
}

void PUT::start_processing()
{
	std::ofstream file;

	get_PUT_status();
	if (_status_code == 201 || _status_code == 204)
	{
		file.open(_status_code == 204 ? _output.path_to_file : (_output.path_to_file = _output.root + _handler.getUrl()), std::ios_base::trunc);
		file << _body;
		file.close();
	}
	else
		_body = get_page_text(_config._error_pages[_status_code]);
	get_hendler();
}

void PUT::get_hendler()
{
	std::stringstream output;

	output << "HTTP/1.1 " << _status_code << " " << _mime.get_error(_status_code) << "\r\n";
	output << get_content_location();
	output << get_server_name();
	output << get_content_length();
	output << get_date_handler();
	output << get_content_type();
	if (_status_code == 405)
		output << get_allow_metods();
	output << "\r\n";
	_head = output.str();
}

std::string PUT::get_content_length()
{
	std::string tmp;

	tmp = "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	return tmp;
}

std::string PUT::get_content_location()
{
	std::string tmp;

	if (_status_code == 201 || _status_code == 204)
		tmp = "Content-Location: " + _handler.getUrl() + "\r\n";
	return tmp;
}