#include "Parser.hpp"
#include "Search_by_configuration.hpp"

std::string Search_by_configuration::create_autoindex_page(t_params &params, Parse_input_handler &handler)
{
	std::stringstream autoindex;
	std::stringstream buff;
	DIR *directory = nullptr;
	char time_buff[1024];
	struct dirent *elem = nullptr;
	struct stat file_stat = {};
	struct tm *time = nullptr;
	std::string tmp_path;

	autoindex << "<html>\n<head><title>Index of " + handler.getUrl() + "</title></head>\n<body bgcolor=\"white\">\n<h1>Index of " +
				 handler.getUrl() + "</h1><hr><pre><a href=\"../\">../</a>\n";

#ifdef AUTOINDEX_DEBUG
	std::cout << autoindex.str();
#endif
	if (!params.root.empty())
	{
		tmp_path = params.root + handler.getUrl();
		directory = opendir(tmp_path.c_str());
	}
	else
	{
		tmp_path = params.alias + ((tmp_path.find("/") != 0 && tmp_path.size()) ? "/" : "") + handler.getUrl().substr(
				params.curent_location.find("*") != std::string::npos ? handler.getUrl().find_last_of("/") : params.curent_location.size());
		directory = opendir(tmp_path.c_str());
	}
	if (!directory)
	{//		elem = readdir(directory); //TODO
		return "";
	}
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
		if (!(stat((tmp_path + elem->d_name).c_str(), &file_stat)))
		{
			autoindex << "<a href=" << elem->d_name << (S_ISDIR(file_stat.st_mode) ? "/" : "") << ">" << elem->d_name
					  << (S_ISDIR(file_stat.st_mode) ? "/" : "") << "</a>";
			for (int i = 0; i < static_cast<int>(51 - std::strlen(elem->d_name) - (S_ISDIR(file_stat.st_mode) ? 1 : 0)); ++i)
				autoindex << " ";
			time = localtime(&file_stat.st_mtim.tv_sec);
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
	closedir(directory);
	return autoindex.str();
}