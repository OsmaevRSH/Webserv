#include "master.hpp"

std::string get_text(const std::string &path_to_file)
{
	std::ifstream	ifs(path_to_file.c_str());
	std::string		text;

	getline(ifs, text, '\0');
	ifs.close();

	return (text);
}

std::string get_page_text(const std::string &path_to_file)
{

	std::string		text;

	if (path_to_file.find(".html") != std::string::npos)
	{
		std::ifstream	ifs(path_to_file.c_str());

		getline(ifs, text, '\0');
		ifs.close();
	}
	else
	{
		std::ifstream input(path_to_file.c_str());
		input.unsetf(std::ios::skipws);
		std::istream_iterator<unsigned char> tmp(input);
		for (; tmp != std::istream_iterator<unsigned char>(); ++tmp)
			text += *tmp;
	}
	return (text);
}