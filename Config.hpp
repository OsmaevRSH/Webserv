/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/19 15:08:32 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include <vector>
#include <map>

/*
**	 Значит, в этом классе мы соберем, обработаем и будем хранить всю
**	информацию из конфигурационного файла, а также, будет ряд методов, которые будут обрабатывать запросы.
**	
**	Мое видение конфига:
**	◦choose the port and host of each "server" - будут две отдельные директивы ip и port вместо listen (так удобнее парсить)
**	◦setup default error pages - область определения не указана, значит будет задаваться только в мейне
**	◦limit client body size - область определения не указана, значит тоже будет задаваться только в мейне
**	◦setup routes with one or multiple of the following rules/configuration (routeswont be using regexp) - все,
**	что будет в подпунктах этого пункта, будет задаваться только в routs (аналог location). Это разрешенные методы запросов,
**	root, autoindex, дефолтный файл ответа, если запрос - директория, все что связано с cgi.
**
**	Конечно, будут еще разные директивы, типо index. Но мне еще предстоит разобраться, как они работают.
*/


class Config
{
	private:
		class _Route
		{
			public:
				std::string					route_text;
				std::vector<std::string>	allow_methods;
				std::string					root;
				bool						autoindex;
				std::string					req_is_dir;
				std::vector<_Route>			routes;
				_Route();
		};
		class _Server
		{
			public:
				std::string 				ip;
				int							port;
				std::string					server_name;
				std::vector<_Route>			routes;
				_Server();
		};
		std::vector<_Server>				_servers;		
		std::map<int, std::string>			_error_pages; // Ключ - номер страницы. Значение - путь
		size_t								_client_body_size;
		std::string							_config_text;	// Сырой текст конфига
	public:
		Config(const std::string& path_to_config);
};