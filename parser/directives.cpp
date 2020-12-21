/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:14:50 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/21 19:40:16 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include "Config.hpp"

void	Config::			select_directive(const std::string &directive, 
									const std::string &dir_content, 
									Config::_Everywhere &ew) 
{
	/* 
	if (directive == "error_page")
	else if (directive == "index")
	else if (directive == "max_body_size")
	else if (directive == "root")
	else if (directive == "autoindex")
	else if (directive == "error_page")
	*/
}

void	Config::_Server ::	select_directive(const std::string &directive, 
									const std::string &dir_content, 
									Config::_Everywhere &ew, 
									Config::_Server &server)
{
	/*
	else if (directive == "index")
	else if (directive == "max_body_size")
	else if (directive == "root")
	else if (directive == "autoindex")
	else if (directive == "ip")
	else if (directive == "port")
	else if (directive == "server_name")
	*/
}

void	Config::_Route::	select_directive(const std::string &directive, 
									const std::string &dir_content, 
									Config::_Everywhere &ew, 
									Config::_Route &route)
{
	/*
	else if (directive == "index")
	else if (directive == "max_body_size")
	else if (directive == "root")
	else if (directive == "autoindex")
	else if (directive == "allow")
	*/
}
