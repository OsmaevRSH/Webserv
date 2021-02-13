#include "master.hpp"

std::string get_page_text(const std::string &path_to_file)
{
	std::ifstream	ifs(path_to_file.c_str());
	std::string		text;

	getline(ifs, text, '\0');
	ifs.close();
	return (text);
}