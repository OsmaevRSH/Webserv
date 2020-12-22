/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:14:50 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 15:44:25 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include "Config.hpp"

void	Config::			select_directive(const std::string &directive, 
									const std::string &dir_content, 
									Config::_Everywhere &ew, size_t base_pos) 
{
	/* 
	if (directive == "error_page")
	else if (directive == "index")
		index_parse(ew, dir_content);
	else if (directive == "max_body_size")
	else if (directive == "root")
	else if (directive == "autoindex")
	else if (directive == "error_page")
	*/
}

void	Config::_Server ::	select_directive(const std::string &directive, 
									const std::string &dir_content, 
									Config::_Everywhere &ew, 
									Config::_Server &server, const size_t base_pos)
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
									Config::_Route &route, const size_t base_pos)
{
	/*
	else if (directive == "index")
	else if (directive == "max_body_size")
	else if (directive == "root")
	else if (directive == "autoindex")
	else if (directive == "allow")
	*/
}

/* во всех функциях нужно будет исправить кучу ошибок */

void	autoindex_parse(Config::_Everywhere &ew, std::string &dir_content, const size_t base_pos)
{
	size_t pos = 0;
	next_word_pos(dir_content, pos);
	std::string value = dir_content.substr(pos, word_len_simple_dir(dir_content, pos));
	if (value == 'on')
		ew.autoindex = true;
	else if (value == 'off')
		ew.autoindex = false;
	else
		show_error(dir_content, pos);
}

void	root_parse(Config::_Everywhere &ew, std::string &dir_content. const size_t base_pos)
{
	size_t pos = 0;
	next_word_pos(dir_content, pos);
	std::string value = dir_content.substr(pos, word_len_simple_dir(dir_content, pos));
}

void	max_body_size_parse(Config::_Everywhere &ew, std::string &dir_content, const size_t base_pos)
{
	size_t pos = 0;
	next_word_pos(dir_content, pos);
	std::string value = dir_content.substr(pos, word_len_simple_dir(dir_content, pos));
	ew._max_body_size = atoi(value.c_str());
}

void	index_parse(Config::_Everywhere &ew, std::string &dir_content, const size_t base_pos)
{
	size_t pos = 0;
	while (next_word_pos(dir_content, pos))
	{
		ew._index.push_back(dir_content.substr(pos, word_len(dir_content, pos)));
		pos += word_len(dir_content, pos);
	}
}

