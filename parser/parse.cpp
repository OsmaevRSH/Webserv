/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 18:28:18 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 20:02:10 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void	Config::parse(t_agrs args)
{
	std::string word;
	std::string dir_content;
	std::vector<std::string> context;

	if (args.route)
		context = args.route_context;
	else if (args.server)
		context = args.server_context;
	else
		context = args.main_context;
	while ((word = get_next_word(args.fragment, args.rel_pos)).empty() == false)
	{
		if (std::find(context.begin(), context.end(), word) == context.end())
			show_error(args);
		select_dir(args, word);
	}

}