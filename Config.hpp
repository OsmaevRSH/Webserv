/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/18 20:22:23 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"

/*
**	 Значит, в этом классе мы соберем, обработаем и будем хранить всю
**	информацию из конфигурационного файла.
**	 В конструкторе мы задаем путь, из которого получаем текст всего файла в виде
**	строки std::string.

**	У нас есть простые и блочные директивы.
**	Блочные:
**		server, location
**	Простые: 
**
*/

typedef struct	s_location
{
	std::string	root;
}				t_location;

typedef struct	s_server
{
	std::string	host;			// Тоже самое что и listen в конфиге. Порт и ip разделены двоеточием.
	t_location	*locations;
	// Два отдельных поля, чтобы было удобнее обращаться к ip и порту.
	std::string ip;
	int			port;
}				t_server;

class Config
{
	private:
		std::string	_config_text;
		t_server	*_servers;
		t_server	_default_server;
	public:
		Config(const std::string& path_to_config);
};