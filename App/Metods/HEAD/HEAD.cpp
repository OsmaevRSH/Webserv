#include "HEAD.hpp"

void HEAD::get_page()
{
	this->Search_path();
	if (_output.status_code != 200)
	{
		_output.path_to_file = _config._error_pages[_output.status_code];
		_tmp_body = get_page_text(_config._error_pages[_output.status_code]);
	}
	if (!_output.autoindex_page.empty())
	{
		_output.path_to_file = "index.html";
		_tmp_body = _output.autoindex_page;
	}
	else
		_tmp_body = get_page_text(_output.path_to_file);
}

HEAD::HEAD(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &head, std::string &body)
		: GET(serv, handler, mime, head, body) {}

std::string HEAD::get_content_length()
{
	std::stringstream tmp;

	tmp << "Content-Length: " << _tmp_body.size() << "\r\n";
	return tmp.str();
}