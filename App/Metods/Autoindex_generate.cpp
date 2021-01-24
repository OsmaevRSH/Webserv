#include "../../Config/Parser.hpp"
#include "Search_by_configuration.hpp"

 std::string Search_by_configuration::create_autoindex_page(t_params &params, Parse_input_handler &handler)
{
	std::stringstream autoindex;
	std::stringstream buff;
	DIR *directory;
	char time_buff[1024];
	struct dirent *elem;
	struct stat file_stat = {};
	struct tm *time;

	autoindex << "<html>\n<head><title>Index of " + handler.getUrl() +
				 "</title></head>\n<body bgcolor=\"white\">\n<h1>Index of " +
				 handler.getUrl() + "</h1><hr><pre><a href=\"../\">../</a>\n";

#ifdef AUTOINDEX_DEBUG
	std::cout << autoindex.str();
#endif
	directory = opendir((params.root + handler.getUrl()).c_str());
	elem = readdir(directory);
	while (elem)
	{
		if (!std::strcmp(elem->d_name, ".") || !std::strcmp(elem->d_name, ".."))
		{
			elem = readdir(directory);
			continue;
		}
#ifdef AUTOINDEX_DEBUG
		std::cout << params.root << handler.getUrl() << elem->d_name
				  << std::endl;
#endif
		if (!(stat((params.root + handler.getUrl() +
					elem->d_name).c_str(), &file_stat)))
		{
			autoindex << "<a href=" << elem->d_name
					  << (S_ISDIR(file_stat.st_mode) ? "/" : "") << ">"
					  << elem->d_name << (S_ISDIR(file_stat.st_mode) ? "/" : "")
					  << "</a>";
			for (int i = 0; i < static_cast<int>(51 - std::strlen(elem->d_name) - (S_ISDIR(file_stat.st_mode) ? 1 : 0)); ++i)
				autoindex << " ";
			time = localtime(&file_stat.st_mtimespec.tv_sec);
			std::string format = "%d-%b-%Y %H:%M";
			strftime(time_buff, 1024, format.c_str(), time);
			autoindex << time_buff;
			for (int i = 0; i < static_cast<int>(20 - (S_ISDIR(file_stat.st_mode) ? 1 : std::to_string(file_stat.st_size).size())); ++i)
				autoindex << " ";
			if (S_ISDIR(file_stat.st_mode))
				autoindex << "-" << std::endl;
			else
				autoindex << file_stat.st_size << std::endl;
			elem = readdir(directory);
		}
	}
	autoindex << "</pre><hr></body>\n</html>\n";
	return autoindex.str();
}