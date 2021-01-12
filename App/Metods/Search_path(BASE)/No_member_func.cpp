#include "Path_to_file.hpp"

bool check_slash(Parse_input_handler &handlers)
{
	if (*handlers.getUrl().rbegin() == '/')
		return true;
	return false;
}

template<class T>
ConfigParser::t_location *check_path_with_complete_coincidence(T &param, Parse_input_handler &handlers)
{
	std::vector<ConfigParser::t_location>::iterator it;
	it = param.locations.begin();

	for (; it < param.locations.end(); ++it)
	{
		if (it->block_args[0] == "=")
		{
			if (it->block_args[1] == handlers.getUrl())
				return &(*it);
		}
	}
	return nullptr;
}

void update_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_location &location)
{
	global_params.index = location.ew.index;
	global_params.root = location.ew.root;
	global_params.allow_methods = location.allow_methods;
	global_params.autoindex = location.ew.autoindex;
	global_params.max_body_size = location.ew.max_body_size;
}

bool search_folder(ConfigHandler::t_params &params, Parse_input_handler &handlers)
{
	struct stat check = {};

	if (!stat((params.root + handlers.getUrl()).c_str(), &check))
	{
		if (S_ISDIR(check.st_mode))
			return true;
	}
	return false;
}

void location_sort(std::vector<ConfigParser::t_location> &locations)
{
	int max;

	for (int i = 0; i < static_cast<int>(locations.size()) - 1; i++)
	{
		max = i;
		for (int j = i + 1; j < static_cast<int>(locations.size()); j++)
		{
			if (locations[j].block_args[0] > locations[max].block_args[0])
				max = j;
		}
		std::swap(locations[i], locations[max]);
	}
}

template<class T>
ConfigParser::t_location *check_simple_location(T &param, Parse_input_handler &handlers)
{
	std::vector<ConfigParser::t_location>::iterator it;
	location_sort(param.locations);
	it = param.locations.begin();

	for (; it < param.locations.end(); ++it)
	{
		if (it->block_args[0] != "=")
		{
			if (!std::strncmp(it->block_args[0].c_str(), handlers.getUrl().c_str(), it->block_args[0].size()))
				return &(*it);
		}
	}
	return nullptr;
}

bool search_file(ConfigHandler::t_params &params, Parse_input_handler &handlers)
{
	struct stat check = {};

	if (!stat((params.root + handlers.getUrl()).c_str(), &check))
	{
		if (S_ISDIR(check.st_mode))
		{
			handlers.setUrl(handlers.getUrl() + "/");
			return true;
		}
		if (S_ISREG(check.st_mode))
			return true;
		return false;
	}
	return false;
}

