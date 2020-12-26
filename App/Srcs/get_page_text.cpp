/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_page_text.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 21:00:58 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/17 21:15:24 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shared.hpp"

std::string get_page_text(const std::string &path_to_file)
{
	std::ifstream	ifs(path_to_file.c_str());
	std::string		text;

	getline(ifs, text, '\0');
	ifs.close();
	return (text);
}