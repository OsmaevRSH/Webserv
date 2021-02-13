#include "GET.hpp"

GET::GET(const Serv_conf &serv, const Parse_request_headers &handler, const MIME_ERROR &mime, std::string &head, std::string &body, std::list<Client>::iterator &Iter)
		: Search_by_configuration(serv, handler, mime, Iter), _body(body), _head(head) {}

void GET::get_page()
{
//	size_t len;
	this->Search_path();
//	char *body;
	if (_output.status_code != 200)
		_body = _mime.get_error_page(_output.status_code);
	else if (!_output.autoindex_page.empty())
	{
		_output.path_to_file = "index.html";
		_body = _output.autoindex_page;
	}
	else
		_body = get_page_text(_output.path_to_file);
//	body = get_document(_output.path_to_file, len);
//	std::cout << len << std::endl << body << std::endl;
//	char test[] = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: 15983\r\nServer: Webserver/1.0\r\n\r\n";
//	std::cout << test << body << std::endl;
//	send(4, test, strlen(test), 0);
//	send(4, body, 15983, 0);
}

void GET::get_hendler()
{
	std::stringstream output;

	output << this->get_first_line() << this->get_content_type() << this->get_content_length() << this->get_date_handler()
		   << this->get_server_name() << this->get_last_modified() << this->get_allow_methods() << this->get_content_lang(_handler.getVariableHandlers(), _body) << "\r\n";
	_head = output.str();
}

void GET::start_processing()
{
	this->get_page();
	this->get_hendler();
}

std::string GET::get_content_length()
{
	std::stringstream tmp;

	tmp << "Content-Length: " << _body.size() << "\r\n";
	return tmp.str();
}
