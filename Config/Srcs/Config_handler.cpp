/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 02:47:25 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/26 02:47:26 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

static void location_sort(std::vector<ConfigParser::t_location> &locations)
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

static void update_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_location &location)
{
	global_params.index = location.ew.index;
	global_params.root = location.ew.root;
	global_params.allow_methods = location.allow_methods;
	global_params.autoindex = location.ew.autoindex;
	global_params.max_body_size = location.ew.max_body_size;
}

static bool check_slash(Parce_input_handler &handlers)
{
	if (*handlers.getUrl().rbegin() == '/')
		return true;
	return false;
}

template<class T>
static ConfigParser::t_location *check_simple_location(T &param, Parce_input_handler &handlers)
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

template<class T>
static ConfigParser::t_location *check_path_with_complete_coincidence(T &param, Parce_input_handler &handlers)
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

static bool search_file(ConfigHandler::t_params &params, Parce_input_handler &handlers)
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

static bool search_folder(ConfigHandler::t_params &params, Parce_input_handler &handlers)
{
	struct stat check = {};

	if (!stat((params.root + handlers.getUrl()).c_str(), &check))
	{
		if (S_ISDIR(check.st_mode))
			return true;
	}
	return false;
}

bool search_index(ConfigHandler::t_params &global_params, Parce_input_handler &handlers)
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
		global_params.autoindex_page = Config::create_autoindex_page(global_params, handlers);
	return false;
}

std::string Config::Handler(t_headers &headers, Parce_input_handler &handlers)
{
	ConfigHandler::t_params	global_params;
	ConfigParser::t_server	curent_server;
	std::string				save_url;

	if (___cache.find(handlers.getUrl()) != ___cache.end())
		return get_page_text(___cache[handlers.getUrl()]);
	else
		save_url = handlers.getUrl();
	setup_global_params(global_params, curent_server, false);
	curent_server = get_server(headers);
	setup_global_params(global_params, curent_server, true);
	global_params.path_to_page = get_path(curent_server, handlers, global_params);
	if (check_allow_metods(global_params, handlers))
		return get_page_text(_error_pages[405]);
	if (!global_params.autoindex_page.empty())
		return global_params.autoindex_page;
	else
	{
		___cache.insert(std::pair<std::string, std::string>(save_url, global_params.path_to_page));
		return get_page_text(global_params.path_to_page);
	}
}

void Config::setup_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_server &server, bool save_server)
{
	if (save_server)
		global_params.root_location = server;
	global_params.index = _ew.index;
	global_params.root = _ew.root;
	global_params.autoindex = _ew.autoindex;
	global_params.max_body_size = _ew.max_body_size;
}

std::string Config::recursive_call_with_slash(Parce_input_handler &handlers, ConfigHandler::t_params &global_params)
{
	if (search_folder(global_params, handlers))
	{
		if (search_index(global_params, handlers))
			return Config::get_path(global_params.root_location, handlers, global_params);
		return _error_pages[404];
	}
	else
		return _error_pages[404];
}

std::string Config::recursive_call_without_slash(Parce_input_handler &handlers, ConfigHandler::t_params &global_params)
{
	if (search_file(global_params, handlers))
	{
		if (check_slash(handlers))
			return Config::get_path(global_params.root_location, handlers, global_params);
		return global_params.root + handlers.getUrl();
	}
	else
		return _error_pages[404];
}

template<class T>
std::string Config::get_path(T &param, Parce_input_handler &handlers, ConfigHandler::t_params &global_params)
{
	ConfigParser::t_location *location;
	std::string tmp;

	if (check_slash(handlers))
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Config::recursive_call_with_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Config::recursive_call_with_slash(handlers, global_params);
			if ((tmp = Config::get_path(*location, handlers, global_params)) == _error_pages[404])
				return Config::recursive_call_with_slash(handlers, global_params);
			return tmp;
		}
		else
			return _error_pages[404];
	}
	else
	{
		if ((location = check_path_with_complete_coincidence(param, handlers)))
		{
			update_global_params(global_params, *location);
			return Config::recursive_call_without_slash(handlers, global_params);
		}
		if ((location = check_simple_location(param, handlers)))
		{
			update_global_params(global_params, *location);
			if (location->locations.empty())
				return Config::recursive_call_without_slash(handlers, global_params);
			if ((tmp = Config::get_path(*location, handlers, global_params)) == _error_pages[404])
				return Config::recursive_call_without_slash(handlers, global_params);
			return tmp;
		}
		else
			return _error_pages[404];
	}
}

ConfigParser::t_server Config::get_server(t_headers &headers)
{
	std::vector<ConfigParser::t_server>::iterator it;
	it = _servers.begin();
	for (; it < _servers.end(); ++it)
	{
		if (it->ip == "127.0.0.1" || it->ip == "localhost")
		{
			if ("127.0.0.1:" + std::to_string(it->port) == headers.Host ||
				"localhost:" + std::to_string(it->port) == headers.Host)
			{
				return *it;
			}
		}
		if (it->ip + std::to_string(it->port) == headers.Host)
		{
			return *it;
		}
	}
	std::cout << "Invalid server!" << std::endl;
	exit(EXIT_FAILURE); //TODO
}








/*
 * -1) Принимаем структуру с полями запроса. И обрабтываем ее тут.
 *
 * 0)	У нас есть структура со всеми полями типо root, index, autoindex и т.д...
 * 		Заходя глубже в блочные директивы, обработчик будет менять эти поля на более приорететные.
 * 		Дойдя до конечной точки, мы на основании содержимого этой структуры принимаем решение, что отдать клиенту.
 *
 * 1)	Ищем сервера с соответствующим ip и портом.
 * 		Если не находим, то выдаем ошибку.
 * 		Что, если есть сервера без указанного ip?
 *
 * 2)	Ищем среди серверов совпадение по server_name. Если не находим, выбираем первый.
 *
 * 3)	Ищем совпадение по маршрутам (location), и если маршрут имеет подмаршруты, углубляемся в них.
 * 		Если маршрут имеет первый аргумент блочной директивы '=' и он совпадает с строкой запроса
 * 		без аргументов, то мы останавливаемся на этом маршруте и дальше не ищем. В иных случаях,
 * 		мы запоминаем максимально совпадающий маршрут (но меньшей или равной длины),
 * 		и если нет других на этом же уровне, то выбираем его. Вложенные маршруты задаются не
 * 		относительно родительского.
 *		Есть ньюанс, если нам прислали директиву, и она обрабатывается индексом. Тогда запрос
 *		обрабатывается заново с соответствующим индексным файлом.
 *
 * 4)	И вот, у нас есть структура с такими полями:
 *		path_to_page
 *		ip
 *		port
 *		server_name
 *		allow_methods
 *		max_body_size
 *		error_pages
 *
 *	5)	Далее смотрим, разрешен ли тот метод, который был в запросе.
 *		Смотрим, есть ли такая страница.
 *		Смотрим, не больше ли content-length чем max_body_size.
 *		В случае ошибки, ищем соответствующую страницу ошибки среди error_pages.
 *		Либо возвращаем стандартную.
 */
