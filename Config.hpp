/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/18 20:55:26 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include <vector>
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

typedef struct					s_location
{
	std::string					root;
	std::vector<std::string>	allow_methods;
}								t_location;

typedef struct					s_server
{
	std::string					listen;			// Тоже самое что и listen в конфиге. Порт и ip разделены двоеточием.
	std::vector<t_location>		locations;
	std::string					server_name;	// Сравниваем с host заголовком в реквесте.
	// Два отдельных поля, чтобы было удобнее обращаться к ip и порту.
	std::string 				ip;
	int							port;
}								t_server;

class Config
{
	private:
		std::string				_config_text;	// Сырой текст конфига
		std::vector<t_server>	_servers;		
		t_server				_def_server;
		std::string				_def_err_page;
		std::string 			_request_is_dir; // Файл, который будет возвращаться, если реквест есть директория
		size_t					_limit_client_body_size;
		bool					_autoindex;
	public:
		Config(const std::string& path_to_config);
};