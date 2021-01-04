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

std::string Config::Handler(t_headers &headers, Input_handlers &handlers)
{
	ConfigParser::t_server curent_server;
	ConfigParser::t_location curent_location;

	curent_server = get_server(headers);
	return get_page_text(location_searcher(curent_server.locations, headers, handlers));
}

std::string Config::location_searcher(std::vector<ConfigParser::t_location> &locations, t_headers &headers, Input_handlers &handlers)
{
	ConfigParser::t_location *curent_location;
	std::string tmp;
	if (!(curent_location = full_match_location(locations, headers, handlers)))
	{
		location_sort(locations);
		if (!(curent_location = simple_location(locations, headers, handlers)))
			return "Error";
		if (curent_location->locations.empty())
			return (curent_location->ew.root + "/" + curent_location->ew.index[0]);
		else
		{
			if ((tmp = location_searcher(curent_location->locations, headers, handlers)) == "Error")
				return (curent_location->ew.root + "/" + curent_location->ew.index[0]);
			else
				return tmp;
		}
	}
	return (curent_location->ew.root + "/" + curent_location->ew.index[0]);
}

void Config::location_sort(std::vector<ConfigParser::t_location> &locations)
{
	int max;
	
	for (int i = 0; i < locations.size() - 1; i++)
	{
		max = i;
		for (int j = i + 1; j < locations.size(); j++)
		{
			if (locations[j].block_args[0] > locations[max].block_args[0])
				max = j;
		}
		std::swap(locations[i], locations[max]);
	}
}

ConfigParser::t_location *Config::simple_location(std::vector<ConfigParser::t_location> &server, t_headers &headers, Input_handlers &handlers)
{
	std::vector<ConfigParser::t_location>::iterator it;
	it = server.begin();
	for (; it < server.end(); ++it)
		if (!std::strncmp(it->block_args[0].c_str(), handlers.getUrl().c_str(), it->block_args[0].size()) && it->block_args[0] != "=")
			return &(*it);
	return nullptr;
}

ConfigParser::t_location *Config::full_match_location(std::vector<ConfigParser::t_location> &server, t_headers &headers, Input_handlers &handlers)
{
	std::vector<ConfigParser::t_location>::iterator it;
	it = server.begin();
	for (; it < server.end(); ++it)
	{
		if (it->block_args[0] == "=")
		{
			if (it->block_args[1] == handlers.getUrl())
			{
				return &(*it);
			}
		}
	}
	return nullptr;
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
				return *it;
		}
		if (it->ip + std::to_string(it->port) == headers.Host)
			return *it;
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
 *
 */
