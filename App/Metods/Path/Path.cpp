#include "Path.hpp"

bool check_slash(Parse_input_handler &handlers)
{
	if (*handlers.getUrl().rbegin() == '/')
		return true;
	return false;
}

bool search_index(t_params &global_params, Parse_input_handler &handlers)
{
	struct stat check = {};
	std::vector<std::string>::iterator it;
	it = global_params.index.begin();

	for (; it < global_params.index.end(); ++it)
	{
		if (!stat((global_params.root + handlers.getUrl() + *it).c_str(), &check))
		{
			handlers.setUrl(handlers.getUrl() + *it);
			return true;
		}
	}
	if (global_params.autoindex)
		global_params.autoindex_page = Path::create_autoindex_page(global_params, handlers);
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

void update_global_params(t_params &global_params, ConfigParser::t_location &location)
{
	global_params.index = location.ew.index;
	global_params.root = location.ew.root;
	global_params.allow_methods = location.allow_methods;
	global_params.autoindex = location.ew.autoindex;
	global_params.max_body_size = location.ew.max_body_size;
}

bool search_folder(t_params &params, Parse_input_handler &handlers)
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

bool search_file(t_params &params, Parse_input_handler &handlers)
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

ConfigParser::t_server Path::get_server()
{
	std::vector<ConfigParser::t_server>::iterator it;
	it = _config._servers.begin();
	for (; it < _config._servers.end(); ++it)
	{
		if (it->ip == "127.0.0.1" || it->ip == "localhost")
		{
			if ("127.0.0.1:" + std::to_string(it->port) == _handler.getVariableHandlers().at("Host") ||
				"localhost:" + std::to_string(it->port) == _handler.getVariableHandlers().at("Host"))
			{
				return *it;
			}
		}
		if (it->ip + std::to_string(it->port) == _handler.getVariableHandlers().at("HOST"))
		{
			return *it;
		}
	}
	std::cout << "Invalid server!" << std::endl;
	exit(EXIT_FAILURE); //TODO
}

void Path::setup_global_params(t_params &global_params, ConfigParser::t_server &server, bool save_server)
{
	if (save_server)
		global_params.root_location = server;
	global_params.index = _config._ew.index;
	global_params.root = _config._ew.root;
	global_params.autoindex = _config._ew.autoindex;
	global_params.max_body_size = _config._ew.max_body_size;
}

std::string Path::recursive_call_with_slash(Parse_input_handler &handlers, t_params &global_params)
{
	if (search_folder(global_params, handlers))
	{
		if (search_index(global_params, handlers))
			return Path::get_path(global_params.root_location, handlers, global_params);
		return _config._error_pages[404];
	}
	else
		return _config._error_pages[404];
}

std::string Path::recursive_call_without_slash(Parse_input_handler &handlers, t_params &global_params)
{
	if (search_file(global_params, handlers))
	{
		if (check_slash(handlers))
			return Path::get_path(global_params.root_location, handlers, global_params);
		return global_params.root + handlers.getUrl();
	}
	else
		return _config._error_pages[404];
}

bool Path::check_allow_metods(const t_params &param, Parse_input_handler &handlers)
{
	if (param.allow_methods.empty())
		return false;
	else
	{
		for (int i = 0; i < static_cast<int>(param.allow_methods.size()); ++i)
		{
			if (param.allow_methods[i] == handlers.getType())
				return false;
		}
		return true;
	}
}

std::string Path::Search_path()
{
	t_params	global_params;
	ConfigParser::t_server	curent_server;
	std::string				save_url;

	if (_config._cache.find(_handler.getUrl()) != _config._cache.end())
		return _config._cache[_handler.getUrl()];
	else
		save_url = _handler.getUrl();
	setup_global_params(global_params, curent_server, false);
	curent_server = get_server();
	setup_global_params(global_params, curent_server, true);
	global_params.path_to_page = get_path(curent_server, _handler, global_params);
	if (check_allow_metods(global_params, _handler))
		return _config._error_pages[405];
	if (!global_params.autoindex_page.empty())
		return global_params.autoindex_page;
	else
	{
		_config._cache.insert(std::pair<std::string, std::string>(save_url, global_params.path_to_page));
		return global_params.path_to_page;
	}
}

template<class T>
std::string Path::get_path(T &param, Parse_input_handler &handlers, t_params &global_params)
{
	ConfigParser::t_location *location;
	std::string tmp;

	if (check_slash(handlers))
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Path::recursive_call_with_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Path::recursive_call_with_slash(handlers, global_params);
			if ((tmp = Path::get_path(*location, handlers, global_params)) == _config._error_pages[404])
				return Path::recursive_call_with_slash(handlers, global_params);
			return tmp;
		}
		else
			return _config._error_pages[404];
	}
	else
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Path::recursive_call_without_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Path::recursive_call_without_slash(handlers, global_params);
			if ((tmp = Path::get_path(*location, handlers, global_params)) == _config._error_pages[404])
				return Path::recursive_call_without_slash(handlers, global_params);
			return tmp;
		}
		else
			return _config._error_pages[404];
	}
}