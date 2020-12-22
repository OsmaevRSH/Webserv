/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:53:16 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 19:37:39 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

static size_t		block_directive(const std::string &text, const size_t &pos)
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
	}
	return std::string::npos;
}

static size_t		simple_directive(const std::string &text, const size_t &pos)
{
	size_t pos_end;

	pos_end = text.find(';', pos);
	return pos_end;
}

std::string				dir_content(const std::string &text, const size_t &pos)
{
	size_t end_pos;
	size_t simple = 0;
	size_t block = 0;
	simple = text.find(';', pos);
	block = text.find('{', pos);
	if (simple == block)
		show_error(text, pos);
	if (block < simple)
		end_pos = block_directive(text, pos);
	else
		end_pos = simple_directive(text, pos);
	return (text.substr(pos, end_pos - pos));
}