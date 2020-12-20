/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/20 17:53:16 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/20 18:24:10 by jeldora          ###   ########.fr       */
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

size_t			word_len(const std::string& str, const size_t &pos)
{
	size_t length = 0;

	while (pos + length < str.length() && (	str[pos + length] != ' ' || 
											str[pos + length] != '{'|| 
											str[pos + length] != '\n'))
		length++;
	return length;
}