#include "Search_by_configuration.hpp"

bool check_slash(Parse_request_headers &handlers)
{
	if (*handlers.getUrl().rbegin() == '/')
		return true;
	return false;
}

bool Search_by_configuration::search_index(t_params &global_params, Parse_request_headers &handlers)
{
	struct stat check = {};
	std::string tmp_path;
	std::vector<std::string>::iterator it;
	it = global_params.index.begin();

	for (; it < global_params.index.end(); ++it)
	{
		if (!global_params.root.empty())
		{
			if (!stat((global_params.root + handlers.getUrl() + *it).c_str(), &check))
			{
				handlers.setUrl(handlers.getUrl() + *it);
				return true;
			}
		}
		else
		{
			tmp_path = handlers.getUrl().substr(global_params.curent_location.find("*") != std::string::npos ? handlers.getUrl().find_last_of("/")
			                                                                                          : global_params.curent_location.size());
			if (tmp_path.find("/") != 0 && tmp_path.size())
				tmp_path = "/" + tmp_path;
			if (!stat((global_params.alias + tmp_path + *it).c_str(), &check))
			{
				handlers.setUrl(handlers.getUrl() + *it);
				return true;
			}
			return false;
		}
	}
	if (global_params.autoindex)
	{
		_output.status_code = 200;
		_output.autoindex_page = Search_by_configuration::create_autoindex_page(global_params, handlers);
	}
	return false;
}

template<class T>
t_location *check_path_with_complete_coincidence(T &param, Parse_request_headers &handlers)
{
	std::vector<t_location>::iterator it;
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

t_location *check_utils(std::vector<t_location>::iterator &it, std::string &regex, std::string &url)
{
	if (it->block_args[0] == "\\")
	{
		int star = 0;
		int reg_i = 0;
		int url_i = 0;
		int url_after_star = 0;

		if (it->block_args.size() >= 2)
			regex = it->block_args[1];
		else
			return nullptr;
		for (; url_i < url.length();)
		{
			if (reg_i == 0 && regex.find('*', reg_i) == std::string::npos)
				return nullptr;
			if ((star = regex.find('*', reg_i)) == std::string::npos)
				star = regex.length();
			std::string s1 = regex.substr(reg_i, star - reg_i);
			std::string s2 = url.substr(url_i, star - reg_i);
			if (s1 != s2)
				return nullptr;
			if (star + 1 > it->block_args[1].length())
				return &(*it);
			url_after_star = url.find(regex[star + 1], url_i);
//			if (url_after_star == std::string::npos)
//				return nullptr;
			url_i = url_after_star;
			reg_i = star + 1;
		}
		return &(*it);
	}
	return nullptr;
}

template<class T>
t_location *check_path_with_simple_regex(T &param, Parse_request_headers &handlers)
{
	t_location *result = nullptr;
	std::vector<t_location>::iterator it = param.locations.begin();
	std::string regex;
	std::string url = handlers.getUrl();

	for (; it < param.locations.end(); ++it)
	{
		if (!(result = check_utils(it, regex, url)))
			continue;
		else
			return result;
	}
	return nullptr;
}

void Search_by_configuration::update_global_params(t_params &global_params, t_location &location)
{
	global_params.index = location.ew.index;
	if (location.block_args[0] == "=" || location.block_args[0] == "\\")
		global_params.curent_location = location.block_args[1];
	else
		global_params.curent_location = location.block_args[0];
	global_params.root = location.ew.root;
	global_params.alias = location.ew.alias;
	global_params.allow_methods = location.allow_methods;
	global_params.autoindex = location.ew.autoindex;
	global_params.max_body_size = location.ew.max_body_size;
	_output.location = location;
}

bool search_folder(t_params &params, Parse_request_headers &handlers)
{
	struct stat check = {};
	std::string tmp_path;

	if (!params.root.empty())
	{
		if (!stat((params.root + handlers.getUrl()).c_str(), &check))
		{
			if (S_ISDIR(check.st_mode))
				return true;
		}
		return false;
	}
	else
	{
		tmp_path = handlers.getUrl().substr(params.curent_location.find("*") != std::string::npos ? handlers.getUrl().find_last_of("/")
																								  : params.curent_location.size());
		if (tmp_path.find("/") != 0 && tmp_path.size())
			tmp_path = "/" + tmp_path;
		if (!stat((params.alias + tmp_path).c_str(), &check))
		{
			if (S_ISDIR(check.st_mode))
				return true;
		}
		return false;
	}
}

void location_sort(std::vector<t_location> &locations)
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
t_location *check_simple_location(T &param, Parse_request_headers &handlers)
{
	std::vector<t_location>::iterator it;
	location_sort(param.locations);
	it = param.locations.begin();

	for (; it < param.locations.end(); ++it)
	{
		if (it->block_args[0] != "=" && it->block_args[0] != "\\")
		{
			if (!std::strncmp(it->block_args[0].c_str(), handlers.getUrl().c_str(), it->block_args[0].size()))
				return &(*it);
		}
	}
	return nullptr;
}

bool search_file(t_params &params, Parse_request_headers &handlers)
{
	struct stat check = {};
	std::string tmp_path;

	if (!params.root.empty())
	{
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
	else
	{
		tmp_path = handlers.getUrl().substr(params.curent_location.find("*") != std::string::npos ? handlers.getUrl().find_last_of("/")
																								  : params.curent_location.size());
		if (tmp_path.find("/") != 0 && tmp_path.size())
			tmp_path = "/" + tmp_path;
		if (!stat((params.alias + tmp_path).c_str(), &check))
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
}

t_server Search_by_configuration::get_server()
{
	std::string port;

	port = _handler.getVariableHandlers().at("Host").substr(_handler.getVariableHandlers().at("Host").find(':'));
	_output.port = std::stoi(port.substr(1));
	std::vector<t_server>::iterator it;
	it = _config._servers.begin();
	for (; it < _config._servers.end(); ++it)
	{
		if (it->ip == "127.0.0.1" || it->ip == "localhost")
		{
			if ("127.0.0.1:" + std::to_string(it->port) == _handler.getServerIp() + port ||
				"localhost:" + std::to_string(it->port) == _handler.getServerIp() + port)
				return *it;
		}
		if (it->ip + ":" + std::to_string(it->port) == _handler.getServerIp() + port)
			return *it;
	}
	return _config._servers[0];
}

void Search_by_configuration::setup_global_params(t_params &global_params, t_server &server, bool save_server)
{
	if (save_server)
		global_params.root_location = server;
	global_params.index = _config._ew.index;
	global_params.root = _config._ew.root;
	global_params.alias = _config._ew.alias;
	global_params.autoindex = _config._ew.autoindex;
	global_params.max_body_size = _config._ew.max_body_size;
}

void Search_by_configuration::recursive_call_with_slash(Parse_request_headers &handlers, t_params &global_params)
{
	if (search_folder(global_params, handlers) && Search_by_configuration::search_index(global_params, handlers))
		return Search_by_configuration::get_path(global_params.root_location, handlers, global_params);
	if (_output.autoindex_page.empty())
		_output.status_code = 404;
}

void Search_by_configuration::recursive_call_without_slash(Parse_request_headers &handlers, t_params &global_params)
{
	std::string tmp_path;

	if (search_file(global_params, handlers))
	{
		if (check_slash(handlers))
			return Search_by_configuration::get_path(global_params.root_location, handlers, global_params);
		_output.path_to_file = global_params.root + handlers.getUrl();

		if (!global_params.root.empty())
			_output.path_to_file = global_params.root + handlers.getUrl();
		else
		{
			tmp_path = handlers.getUrl().substr(global_params.curent_location.find("*") != std::string::npos ? handlers.getUrl().find_last_of("/")
			                                                                                          : global_params.curent_location.size());
			if (tmp_path.find("/") != 0 && tmp_path.size())
				tmp_path = "/" + tmp_path;
			_output.path_to_file = global_params.alias + tmp_path;
		}
	}
	else
		_output.status_code = 404;
}

void Search_by_configuration::check_allow_metods(const t_params &param, Parse_request_headers &handlers)
{
	if (!param.allow_methods.empty())
	{
		for (int i = 0; i < static_cast<int>(param.allow_methods.size()); ++i)
		{
			if (param.allow_methods[i] == handlers.getType())
				return;
		}
		_output.status_code = 405;
	}
}

void Search_by_configuration::check_body_size(const t_params &param, Parse_request_headers &handlers)
{
	if (param.max_body_size && _iter->_request_body.length() > param.max_body_size)
		_output.status_code = 413;
}

void Search_by_configuration::Search_path()
{
	t_params global_params;
	t_server curent_server;

	_output.status_code = 200;
	setup_global_params(global_params, curent_server, false);
	curent_server = get_server();
	setup_global_params(global_params, curent_server, true);
	get_path(curent_server, _handler, global_params);
	check_body_size(global_params, _handler);
	check_allow_metods(global_params, _handler);
	_output.root = global_params.root;
	_output.alias = global_params.alias;
	_output.curent_location = global_params.curent_location;
}

template<class T>
void Search_by_configuration::get_path(T &param, Parse_request_headers &handlers, t_params &global_params)
{
	t_location *location;

	if (check_slash(handlers))
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Search_by_configuration::recursive_call_with_slash(handlers, global_params);
		}
		if ((location = check_path_with_simple_regex(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Search_by_configuration::recursive_call_with_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Search_by_configuration::recursive_call_with_slash(handlers, global_params);
			_output.attached_location = true;
			Search_by_configuration::get_path(*location, handlers, global_params);
			if (!_output.attached_location)
				return Search_by_configuration::recursive_call_with_slash(handlers, global_params);
		}
		else
		{
			if (!_output.attached_location)
				_output.status_code = 404;
			else
				_output.attached_location = false;
		}
	}
	else
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Search_by_configuration::recursive_call_without_slash(handlers, global_params);
		}
		if ((location = check_path_with_simple_regex(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Search_by_configuration::recursive_call_without_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Search_by_configuration::recursive_call_without_slash(handlers, global_params);
			_output.attached_location = true;
			Search_by_configuration::get_path(*location, handlers, global_params);
			if (!_output.attached_location)
				return Search_by_configuration::recursive_call_without_slash(handlers, global_params);
		}
		else
		{
			if (!_output.attached_location)
				_output.status_code = 404;
			else
				_output.attached_location = false;
		}
	}
}

Search_by_configuration::Search_by_configuration(const Serv_conf &conf, const Parse_request_headers &handler, const MIME_ERROR &mime, std::list<Client>::iterator &iter)
		: _config(conf), _handler(handler), _mime(mime), _iter(iter) {}

Search_by_configuration::~Search_by_configuration() {}