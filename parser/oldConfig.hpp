/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oldConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 16:23:06 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include <vector>
#include <map>
#include <algorithm>

namespace Conf
{


class Config
{
	public:
		class _Everywhere
		{
			public:
				std::vector<std::string>	_index;
				size_t						_max_body_size;
				std::string					_root;
				bool						_autoindex;

				_Everywhere();
		};
		class _Route
		{
			public:
				std::vector<std::string>	_allow_methods;
				std::vector<_Route>			_routes;
				_Everywhere					_ew;

				void	parse(std::string text);
				void	Config::_Route::	select_directive(const std::string &directive, 
									const std::string &dir_content, 
									Config::_Everywhere &ew, 
									Config::_Route &route);

				_Route();
		};
		class _Server
		{
			public:
				std::string 				_ip;
				int							_port;
				std::string					_server_name;
				std::vector<_Route>			_routes;
				_Everywhere					_ew;

				void	parse(std::string text);
				void	select_directive(const std::string &directive, 
										const std::string &dir_content, 
										Config::_Everywhere &ew, 
										Config::_Server &server);

				_Server();
		};
	private:

		std::vector<_Server>				_servers;		
		std::map<int, std::string>			_error_pages; // Ключ - номер страницы. Значение - путь
		_Everywhere							_ew;

		void	parse(std::string text);
		void	parse_server_block(std::vector<std::string> context, std::string text, Config::_Server server);
		void	parse_server(const std::string &dir_content);
		void	parse_route(const std::string &dir_content);
		void	select_directive(const std::string &directive, 
								const std::string &dir_content, 
								Config::_Everywhere &ew);
	public:
		Config(const std::string& path_to_config);
};

static bool				next_word_pos(const std::string& str, size_t &pos);
static size_t			word_len(const std::string& str, const size_t &pos);
void					show_error(const std::string& str, size_t pos);
size_t					end_directive(const std::string &text, const size_t &pos);

}