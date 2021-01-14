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

std::string Path::get_server_name()
{
	std::stringstream tmp;

	tmp << "Server: " << "Webserver/1.0" << "\r\n";
	return tmp.str();
}

std::string Path::get_last_modified()
{
	struct stat buf = {0};
	struct tm *time = nullptr;
	char time_buff[1024];
	std::stringstream tmp;

	if (!(stat(_output.path_to_file.c_str(), &buf)))
	{
		time = localtime(&buf.st_mtimespec.tv_sec);
		std::string format = "%a, %d %b %G %T %Z";
		strftime(time_buff, 1024, format.c_str(), time);
		tmp << "Last-Modified: " << time_buff << "\r\n";
		return tmp.str();
	}
	else
		return "";
}