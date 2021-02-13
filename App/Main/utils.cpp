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

#include "master.hpp"

std::string get_text(const std::string &path_to_file)
{
	std::ifstream	ifs(path_to_file.c_str());
	std::string		text;

	getline(ifs, text, '\0');
	ifs.close();

	return (text);
}

std::string get_page_text(const std::string &path_to_file)
{

	std::string		text;

	if (path_to_file.find(".html"))
	{
		std::ifstream	ifs(path_to_file.c_str());

		getline(ifs, text, '\0');
		ifs.close();
	}
	else{
		std::ifstream input(path_to_file.c_str(), std::ios::binary);
		std::vector<unsigned char> buffer(std::istream_iterator<char>(input), {});

		text = std::string(buffer.begin(), buffer.end());
	}

	return (text);
}