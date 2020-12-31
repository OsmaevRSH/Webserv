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
	ConfigParser::t_route curent_route;

	curent_server = get_server(headers);
//	std::string out = route_searcher(curent_server.routes, headers, handlers);
	return get_page_text(route_searcher(curent_server.routes, headers, handlers));
}

std::string Config::route_searcher(std::vector<ConfigParser::t_route> &routes, t_headers &headers, Input_handlers &handlers)
{
	ConfigParser::t_route *curent_route;
	std::string tmp;
	if (!(curent_route = full_match_route(routes, headers, handlers)))
	{
		route_sort(routes);
		if (!(curent_route = simple_route(routes, headers, handlers)))
			return "Error";
		if (curent_route->routes.empty())
			return (curent_route->ew.root + "/" + curent_route->ew.index[0]);
		else
		{
			if ((tmp = route_searcher(curent_route->routes, headers, handlers)) == "Error")
				return (curent_route->ew.root + "/" + curent_route->ew.index[0]);
			else
				return tmp;
		}
	}
	return (curent_route->ew.root + "/" + curent_route->ew.index[0]);
}

void Config::route_sort(std::vector<ConfigParser::t_route> &routes)
{
	int max;
	
	for (int i = 0; i < routes.size() - 1; i++)
	{
		max = i;
		for (int j = i + 1; j < routes.size(); j++)
		{
			if (routes[j].block_args[0] > routes[max].block_args[0])
				max = j;
		}
		std::swap(routes[i], routes[max]);
	}
}

ConfigParser::t_route *Config::simple_route(std::vector<ConfigParser::t_route> &server, t_headers &headers, Input_handlers &handlers)
{
	std::vector<ConfigParser::t_route>::iterator it;
	it = server.begin();
	for (; it < server.end(); ++it)
		if (!std::strncmp(it->block_args[0].c_str(), handlers.getUrl().c_str(), it->block_args[0].size()) && it->block_args[0] != "=")
			return &(*it);
	return nullptr;
}

ConfigParser::t_route *Config::full_match_route(std::vector<ConfigParser::t_route> &server, t_headers &headers, Input_handlers &handlers)
{
	std::vector<ConfigParser::t_route>::iterator it;
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
			{
				return *it;
			}
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
 * 3)	Ищем совпадение по маршрутам (route), и если маршрут имеет подмаршруты, углубляемся в них.
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
