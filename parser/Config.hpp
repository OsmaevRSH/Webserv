/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/20 20:10:22 by jeldora          ###   ########.fr       */
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
**	Далее, я делаю парсер.
**	
*/



class Config
{
	private:
		class _Everywhere
		{
			public:
				std::string					index;
				size_t						max_body_size;
				std::string					root;
				bool						autoindex;

				_Everywhere();
		};
		class _Route
		{
			public:
				std::vector<std::string>	allow_methods;
				std::vector<_Route>			routes;
				_Everywhere					ew;

				_Route();
		};
		class _Server
		{
			public:
				std::string 				ip;
				int							port;
				std::string					server_name;
				std::vector<_Route>			routes;
				_Everywhere					ew;

				_Server();
		};
		std::vector<_Server>				_servers;		
		std::map<int, std::string>			_error_pages; // Ключ - номер страницы. Значение - путь
		_Everywhere							_ew;
	public:
		Config(const std::string& path_to_config);
};

static bool				next_word_pos(const std::string& str, size_t &pos);
static size_t			word_len(const std::string& str, const size_t &pos);
void					show_error(const std::string& str, size_t pos);
size_t					end_directive(const std::string &text, const size_t &pos);

static void				parse_server();
static void				parse_route();
