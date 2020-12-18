/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:18:58 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/18 19:56:29 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"



Config::Config(const std::string& path_to_config)
{
	_config_text = get_page_text(path_to_config);
}