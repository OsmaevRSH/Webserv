#include "Search_by_configuration.hpp"

std::string Search_by_configuration::get_date_handler()
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

std::string Search_by_configuration::get_first_line()
{
	std::stringstream tmp;

	tmp << "HTTP/1.1 " << _output.status_code << " " << _mime.get_error(_output.status_code) << "\r\n";
	return tmp.str();
}

std::string Search_by_configuration::get_content_type()
{
	std::stringstream tmp;

	tmp << "Content-Type: " << _mime.get_document_types(_output.path_to_file) << "\r\n";
	return tmp.str();
}

std::string Search_by_configuration::get_server_name()
{
	std::stringstream tmp;

	tmp << "Server: " << "Webserver/1.0" << "\r\n";
	return tmp.str();
}

std::string Search_by_configuration::get_last_modified()
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

std::string Search_by_configuration::get_allow_methods()
{
	std::stringstream tmp;
	std::vector<std::string>::iterator it;

	it = _output.location.allow_methods.begin();

	if (it != _output.location.allow_methods.end() && _output.status_code == 405)
	{
		tmp << "Allow: ";
		tmp << *it++;
		for (; it < _output.location.allow_methods.end(); ++it)
			tmp << ", " << *it;
		tmp << "\r\n";
	}
	return tmp.str();
}

std::string Search_by_configuration::get_content_lang(const std::map<std::string, std::string> &headers, const std::string &body)
{
	std::string client_lang;
	std::vector<std::string> page_langs;
	std::string common_lang;
	int 		lang_pos = 0;
	int			count_occur = 0;
	int 		cur_pos = 0;

	if (headers.find("Accept-Language") != headers.end())
		client_lang = (*headers.find("Accept-Language")).second;
	else
		return "";
	if (client_lang == "*" || client_lang.empty())
		return ("Content-Language: *\r\n");
	while (lang_pos != std::string::npos)
	{
		lang_pos = body.find("lang=\"", cur_pos);
		cur_pos += lang_pos + strlen("lang=\"") + 2;
		if (lang_pos == std::string::npos)
			break ;
		page_langs.push_back(body.substr(lang_pos + strlen("lang=\""), 2));
	}
	if (std::find(page_langs.begin(), page_langs.end(), client_lang.substr(0, 2)) != page_langs.end())
		return ("Content-Language: " + client_lang.substr(0, 2) + "\r\n");
	if (page_langs.empty())
		return ("Content-Language: *\r\n");
	for (std::vector<std::string>::iterator i = page_langs.begin(); i != page_langs.end(); ++i) {
		int occur = 0;
		for (std::vector<std::string>::iterator j = page_langs.begin(); j != page_langs.end(); ++j) {
			if (*j == *i)
				occur++;
		}
		if (occur > count_occur) {
			count_occur = occur;
			common_lang = *i;
		}
	}
	return ("Content-Language: " + common_lang + "\r\n");
}
