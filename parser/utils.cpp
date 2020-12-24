/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:53:16 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 23:55:28 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

static std::string		block_directive(const std::string &text, size_t &pos)
{
	size_t start = text.find('{', pos);
	size_t end = start;
	size_t tmp_pos;

	tmp_pos = end;
	while ((end = text.find('}', end)) != std::string::npos)
	{
		tmp_pos = text.rfind('{', --tmp_pos);
		if (tmp_pos == start)
		{
			pos = end++;
			start++;
			end--;
			return text.substr(start, end - start);
		}
	}

	return std::string();
}

static size_t		simple_directive(const std::string &text, size_t &pos)
{
	size_t pos_end;

	pos_end = text.find(';', pos);
	return pos_end;
}

std::string				dir_content(t_args &args)
{
	size_t end_pos;
	size_t simple = 0;
	size_t block = 0;
	simple = args.fragment.find(';', args.rel_pos);
	block = args.fragment.find('{', args.rel_pos);
	if (simple == block)
		show_error(args);
	if (block < simple)
	{
		std::string ret = block_directive(args.fragment, args.rel_pos);
		return (ret);
	}
	else
		end_pos = args.fragment.find(';', args.rel_pos);
	return (args.fragment.substr(args.rel_pos, end_pos - args.rel_pos));
}

