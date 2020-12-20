/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:53:16 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/20 23:46:53 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

void				show_error(const std::string& str, size_t pos)
{
	size_t	line = 1;
	size_t	l_pos = 0;
	size_t	c_pos = 0;

	if (str.rfind('\n', pos) != std::string::npos)
		c_pos = pos - str.rfind('\n', pos);
	else
		c_pos = pos;
	while ((l_pos = str.rfind('\n', pos)) != std::string::npos)
		line++;
	std::cout << "\nParser error: " << l_pos << " line, " << c_pos << " character.\n";
	exit(1);
}

bool				next_word_pos(const std::string& str, size_t &pos)
{
	while (pos < str.length() && (	str[pos] == ' ' || 
									str[pos] == '\n'))
		pos++;
	if (pos == str.length())
		return false;
	return true;
}

size_t				word_len(const std::string& str, const size_t &pos)
{
	size_t length = 0;

	while (pos + length < str.length() && (	str[pos + length] != ' ' || 
											str[pos + length] != '{'|| 
											str[pos + length] != '\n'))
		length++;
	return length;
}

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
		else if (tmp_pos < pos_1)
			show_error(text, pos_2);
	}
	show_error(text, pos_2);
	return std::string::npos;
}

static size_t		simple_directive(const std::string &text, const size_t &pos)
{
	size_t pos_end;

	pos_end = text.find(';', pos);
	if (pos_end == std::string::npos)
		show_error(text, pos);
	return pos_end;
}

size_t				end_of_directive(const std::string &text, const size_t &pos)
{
	size_t simple = 0;
	size_t block = 0;
	simple = text.find(';', pos);
	block = text.find('{', pos);
	if (simple == block)
		show_error(text, pos);
	if (block < simple)
		return block_directive(text, pos);
	else
		return simple_directive(text, pos);
}