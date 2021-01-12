#include "Path_to_file.hpp"
#include "Path.hpp"

ConfigParser::t_server Path_to_file::get_server()
{
	std::vector<ConfigParser::t_server>::iterator it;
	it = _config._servers.begin();
	for (; it < _config._servers.end(); ++it)
	{
		if (it->ip == "127.0.0.1" || it->ip == "localhost")
		{
			if ("127.0.0.1:" + std::to_string(it->port) == _handler.getVariableHandlers().at("HOST") ||
				"localhost:" + std::to_string(it->port) == _handler.getVariableHandlers().at("HOST"))
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

void Path_to_file::setup_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_server &server, bool save_server)
{
	if (save_server)
		global_params.root_location = server;
	global_params.index = _config._ew.index;
	global_params.root = _config._ew.root;
	global_params.autoindex = _config._ew.autoindex;
	global_params.max_body_size = _config._ew.max_body_size;
}

std::string Path_to_file::recursive_call_with_slash(Parse_input_handler &handlers, ConfigHandler::t_params &global_params)
{
	if (search_folder(global_params, handlers))
	{
		if (search_index(global_params, handlers))
			return Path_to_file::get_path(global_params.root_location, handlers, global_params);
		return _config._error_pages[404];
	}
	else
		return _config._error_pages[404];
}

std::string Path_to_file::recursive_call_without_slash(Parse_input_handler &handlers, ConfigHandler::t_params &global_params)
{
	if (search_file(global_params, handlers))
	{
		if (check_slash(handlers))
			return Path_to_file::get_path(global_params.root_location, handlers, global_params);
		return global_params.root + handlers.getUrl();
	}
	else
		return _config._error_pages[404];
}

bool Path_to_file::check_allow_metods(const ConfigHandler::t_params &param, Parse_input_handler &handlers)
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

bool search_index(ConfigHandler::t_params &global_params, Parse_input_handler &handlers)
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
		global_params.autoindex_page = Path_to_file::create_autoindex_page(global_params, handlers);
	return false;
}

std::string Path_to_file::Path()
{
	ConfigHandler::t_params	global_params;
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
std::string Path_to_file::get_path(T &param, Parse_input_handler &handlers, ConfigHandler::t_params &global_params)
{
	ConfigParser::t_location *location;
	std::string tmp;

	if (check_slash(handlers))
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Path_to_file::recursive_call_with_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Path_to_file::recursive_call_with_slash(handlers, global_params);
			if ((tmp = Path_to_file::get_path(*location, handlers, global_params)) == _config._error_pages[404])
				return Path_to_file::recursive_call_with_slash(handlers, global_params);
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
			return Path_to_file::recursive_call_without_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Path_to_file::recursive_call_without_slash(handlers, global_params);
			if ((tmp = Path_to_file::get_path(*location, handlers, global_params)) == _config._error_pages[404])
				return Path_to_file::recursive_call_without_slash(handlers, global_params);
			return tmp;
		}
		else
			return _config._error_pages[404];
	}
}