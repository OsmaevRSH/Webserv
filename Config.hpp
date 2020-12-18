/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/18 23:34:27 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include <vector>
#include <map>
/*
**	 Значит, в этом классе мы соберем, обработаем и будем хранить всю
**	информацию из конфигурационного файла.
**	 В конструкторе мы задаем путь, из которого получаем текст всего файла в виде
**	строки std::string.

**	У нас есть простые и блочные директивы.
**	_Everwhere - все директивы, которые могут храниться везде
**	_Location - директивы, которые могут храниться в location
**	_Server - директивы, которые могут храниться в server
*/

class Config
{
	private:
		class _Everywhere
		{
			public:
				std::string					root;
				size_t						client_body_size;
				std::map<int, std::string>	error_pages; // Ключ - номер страницы. Значение - путь
				bool						autoindex;
				std::string 				request_is_dir; // Файл, который будет возвращаться, если реквест есть директория
				_Everywhere();
		};
		class _Location
		{
			public:
				std::string					value;
				/* Полное совпадение =
				** Максимальное совпадение /
				*/
				std::vector<std::string>	allow_methods;
				std::vector<_Location>		locations;
				_Everywhere					ew;
				// Тут будут еще всякие директивы
				_Location();
		};
		class _Server
		{
			public:
				std::string					listen;			// Тоже самое что и listen в конфиге. Порт и ip разделены двоеточием.
				std::vector<_Location>		locations;
				std::string					server_name;	// Сравниваем с host заголовком в реквесте.
				// Два отдельных поля, чтобы было удобнее обращаться к ip и порту.
				std::string 				ip;
				int							port;
				_Everywhere					ew;
				_Server();
		};
		std::string				_config_text;	// Сырой текст конфига
		std::vector<_Server>	_servers;		
		_Server					_def_server;
	public:
		Config(const std::string& path_to_config);
};