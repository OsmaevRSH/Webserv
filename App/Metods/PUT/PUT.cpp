#include "PUT.hpp"

PUT::PUT(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &head, std::string &handler_body)
		: Search_by_configuration(serv, handler, mime), _head(head), _body(handler_body) {}

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
	get_hendler();
}

void PUT::get_hendler()
{
	std::stringstream output;

	output << "HTTP/1.1 " << _status_code << " " << _mime.get_error(_status_code) << "\r\n";
	if (_status_code == 201 || _status_code == 204)
	{
		output << "Content-Location: " << _handler.getUrl();
	}
	_head = output.str();
}

std::string PUT::get_content_length()
{
	return std::string();
}
