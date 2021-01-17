#include "PUT.hpp"

PUT::PUT(const Serv_conf &serv, const Parse_input_handler &handler, const MIME_ERROR &mime, std::string &body)
		: Search_by_configuration(serv, handler, mime), _body(body) {}

//std::string PUT::get_content_location()
//{
//	std::stringstream tmp;
//
////	tmp <<
//}

std::string PUT::get_PUT_status()
{
	this->Search_path();

	if (_output.status_code == 200)
		_status_code = 204;
	else
		_status_code = 201;
}

void PUT::start_processing()
{
	std::ofstream file;

	get_PUT_status();
	file.open(_status_code == 204 ? _output.path_to_file : _output.location., std::ios_base::trunc);
	if (_status_code == 204)
	{
		std::ofstream file(_output.path_to_file, std::ios_base::trunc);
		file << _handler.getBody();
		file.close();
	}
}

void PUT::get_hendler()
{
	std::stringstream output;

	output << "HTTP/1.1 " <<
}
