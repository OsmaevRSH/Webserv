#include "GET.hpp"

GET::GET(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime)
		: Path(serv, handler, mime){}

std::string GET::get_page()
{
	this->Search_path();
	if (!_output.autoindex_page.empty())
	{
		_output.path_to_file = "index.html";
		_output.status_code = 200;
		return _output.autoindex_page;
	}
	if (_output.status_code != 200 && _output.path_to_file.empty())
	{
		_output.path_to_file = _output.status_code;
		return get_page_text(_config._error_pages[_output.status_code]);
	}
	else
		return get_page_text(_output.path_to_file);
}

std::string GET::get_hendler(const std::string &page)
{
	std::stringstream output;

	output << this->get_first_line() << this->get_content_type() << this->get_content_length(page) << this->get_date_handler() << "\r\n";
	return output.str();
}
