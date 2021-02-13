#include "HEAD.hpp"

void HEAD::get_page()
{
	this->Search_path();
	if (_output.status_code != 200)
	{
		_body = _config._error_pages.find(_output.status_code) == _config._error_pages.end() ? _mime.get_error_page(_output.status_code) : _config._error_pages[_output.status_code];
	}
	else if (!_output.autoindex_page.empty())
	{
		_output.path_to_file = "index.html";
		_tmp_body = _output.autoindex_page;
	}
	else
		_tmp_body = get_page_text(_output.path_to_file);
}

HEAD::HEAD(const Serv_conf &serv, const Parse_request_headers &handler, const MIME_ERROR &mime, std::string &head, std::string &body, std::list<Client>::iterator &Iter)
		: GET(serv, handler, mime, head, body, Iter) {}

std::string HEAD::get_content_length()
{
	std::stringstream tmp;

	tmp << "Content-Length: " << (_output.status_code == 405 ? _body.size() : _tmp_body.size()) << "\r\n";
	return tmp.str();
}