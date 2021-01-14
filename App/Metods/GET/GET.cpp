#include "GET.hpp"

GET::GET(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &head, std::string &body)
		: Path(serv, handler, mime), _body(body), _hendler(head) {}

void GET::get_page()
{
	this->Search_path();
	if (_output.status_code != 200)
	{
		_output.path_to_file = _config._error_pages[_output.status_code];
		_body = get_page_text(_config._error_pages[_output.status_code]);
	}
	if (!_output.autoindex_page.empty())
	{
		_output.path_to_file = "index.html";
		_body = _output.autoindex_page;
	}
	else
		_body = get_page_text(_output.path_to_file);
}

void GET::get_hendler()
{
	std::stringstream output;

	output << this->get_first_line() << this->get_content_type() << this->get_content_length(_body) << this->get_date_handler()
		   << this->get_server_name() << this->get_last_modified() << "\r\n";
	_hendler = output.str();
}

void GET::get_start()
{
	this->get_page();
	this->get_hendler();
}
