/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   directives.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:14:50 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 20:16:41 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"


void	Config::select_dir(t_args &args, std::string word)
{
	t_agrs new_args;
	new_args.text = args.text;
	new_args.ew = args.ew;
	new_args.fragment = dir_content(args.fragment, args.rel_pos);
	new_args.base_pos = args.base_pos + args.rel_pos;
	if (word == "server")
	{
		t_server *server = new t_server;
		new_args.server = server;
		new_args.ew = &(server->ew);
		parse(new_args);
		_servers.push_back(*server);
	}
	else if (word == "route")
	{
		t_route *route = new t_route;
		new_args.route = route;
		new_args.ew = &(route->ew);
		parse(new_args);
		if (args.server)
			args.server->route.push_back(*route);
		else
			args.route->route.push_back(*route);
	}
	else if (word == "autoindex")
	else if (word == "root")
	else if (word == "max_body_size")
	else if (word == "index")


	args.rel_pos += new_args.fragment.length();
}

void	autoindex_parse()
{
	
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

