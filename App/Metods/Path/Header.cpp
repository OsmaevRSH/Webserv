#include "Path.hpp"

std::string Path::get_date_handler()
{
	std::stringstream tmp;
	time_t curent_time;
	tm *time_info;
	std::string format;
	char buffer[80];

	curent_time = time(nullptr);
	time_info = localtime(&curent_time);
	format = "%a, %d %b %G %T %Z";
	strftime(buffer, 80, format.c_str(), time_info);
	tmp << "Date: " << buffer << "\r\n";
	return tmp.str();
}

std::string Path::get_first_line()
{
	std::stringstream tmp;

	tmp << "HTTP/1.1 " << _output.status_code << " " << _mime.get_error(_output.status_code) << "\r\n";
	return tmp.str();
}

std::string Path::get_content_type()
{
	std::stringstream tmp;

	tmp << "Content-Type: " << _mime.get_document_types(_output.path_to_file) << "\r\n";
	return tmp.str();
}

std::string Path::get_content_length(const std::string &page)
{
	std::stringstream tmp;

	tmp << "Content-Length: " << page.size() << "\r\n";
	return tmp.str();
}