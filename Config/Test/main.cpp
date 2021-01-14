/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 02:47:45 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/23 02:51:04 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int		main(void)
{
	Config conf("./Config/conf");
	std::cout <<  "\n" << conf.getServers().front().ip << "\n";
	std::cout << "\n";
	return (0);
}