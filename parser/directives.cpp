/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:14:50 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/21 01:08:50 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include "Config.hpp"

void				Config::select_directive(const std::string &directive, const std::string &dir_content)
{
	if (directive == "server")
		parse_server(dir_content);
	else if (directive == "route")
		parse_route(dir_content);
	/*else if (directive == "error_page")
	else if (directive == "index")
	else if (directive == "max_body_size")
	else if (directive == "root")
	else if (directive == "autoindex")
	else if (directive == "ip")
	else if (directive == "port")
	else if (directive == "server_name")
	else if (directive == "allow")
	*/
}

void				Config::parse_server(const std::string &dir_content)
{
	std::vector<std::string>	server_context;
	_Server						server;

	server_context.push_back("index");
	server_context.push_back("max_body_size");
	server_context.push_back("root");
	server_context.push_back("autoindex");
	server_context.push_back("ip");
	server_context.push_back("port");
	server_context.push_back("server_name");
	server_context.push_back("route");

	parse_block(server_context, dir_content);
}

void				Config::parse_route(const std::string &dir_content)
{
	std::vector<std::string> route_context;

	route_context.push_back("index");
	route_context.push_back("max_body_size");
	route_context.push_back("root");
	route_context.push_back("autoindex");
	route_context.push_back("allow");
	route_context.push_back("route");

	parse_block(route_context, dir_content);
}

