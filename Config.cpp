/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:18:58 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/19 23:34:03 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
**	Как будет устроен парсер? Какие есть крайние случаи?
**	Неправильное слово, 
**
**	Во время парсинга будет переменная, которая будет отслеживать,
**	на какой строке находиться парсер. Если нейдет ошибку, вернет номер строки.

**	Будет несколько модулей парсеров, которые будут вызываться один из другого

**	Итак, что мы ждем в мейне?
**	MAIN: server | error_page | index | max_body_size | root | autoindex ]
**	SERVER: ip | port | server_name | route | [ index | max_body_size | root | autoindex ]
**	ROUTE: allow | route | [ index | max_body_size | root | autoindex ]
*/



Config::Config(const std::string& path_to_config)
{
	_config_text = get_page_text(path_to_config);
}

Config::_Everywhere::_Everywhere()
{
	root = "";
	index = "";
	max_body_size = 0;
	autoindex = false;
}

Config::_Route::_Route()
{
	route_text = "";
}

Config::_Server::_Server()
{
	port = 0;
	ip = "";
	server_name = "";
}