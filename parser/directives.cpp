/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:14:50 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/20 20:08:00 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include "Config.hpp"

void				parse_server()
{
	std::vector<std::string> server_context;

	server_context.push_back("index");
	server_context.push_back("max_body_size");
	server_context.push_back("root");
	server_context.push_back("autoindex");
	server_context.push_back("ip");
	server_context.push_back("port");
	server_context.push_back("server_name");
	server_context.push_back("route");

}

void				parse_route()
{
	std::vector<std::string> route_context;

	route_context.push_back("index");
	route_context.push_back("max_body_size");
	route_context.push_back("root");
	route_context.push_back("autoindex");
	route_context.push_back("allow");
	route_context.push_back("route");
}

static size_t				block_directive(const std::string &text, const size_t &pos)
{
	size_t pos_1 = text.find('{', pos);
	size_t pos_2 = pos_1;
	size_t tmp_pos;

	tmp_pos = pos_2;
	while ((pos_2 = text.find('}', pos_2)) != std::string::npos)
	{
		tmp_pos = text.rfind('{', --tmp_pos);
		if (tmp_pos == pos_1)
			return pos_2;
		else if (tmp_pos < pos_1)
			show_error(text, pos_2);
	}
	show_error(text, pos_2);
	return std::string::npos;
}

static size_t				simple_directive(const std::string &text, const size_t &pos)
{
	size_t pos_end;

	pos_end = text.find(';', pos);
	if (pos_end == std::string::npos)
		show_error(text, pos);
	return pos_end;
}

size_t				end_directive(const std::string &text, const size_t &pos)
{
	size_t simple = 0;
	size_t block = 0;
	simple = text.find(';', pos);
	block = text.find('{', pos);
	if (simple == block)
		show_error(text, pos);
	if (simple == std::string::npos || simple > block)
		return block_directive(text, pos);
	else
		return simple_directive(text, pos);
}



/*
	узнаем тип идрективы
	вырезаем весь контент из нее
	посылаем в функцию обработчик
*/