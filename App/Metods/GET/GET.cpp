#include "GET.hpp"

GET::GET(const Serv_conf &serv, const Parse_input_handler &handler)
		: Path(serv, handler) {}

std::string GET::get_page()
{
	this->Search_path();
	if (!_output.autoindex_page.empty())
		return _output.autoindex_page;
	if (_output.status_code != 200 && _output.path_to_file.empty())
		return get_page_text(_config._error_pages[_output.status_code]);
	else
		return get_page_text(_output.path_to_file);
}
