#include "HEAD.hpp"

std::string HEAD::get_date_handler()
{
	time_t curent_time;
	tm *time_info;
	std::string format;
	char buffer[80];

	curent_time = time(nullptr);
	time_info = localtime(&curent_time);
	format = "%a, %d %b %G %T %Z";
	strftime(buffer, 80, format.c_str(), time_info);
	return buffer;
}