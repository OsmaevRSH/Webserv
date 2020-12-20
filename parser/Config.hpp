/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:15:56 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/21 00:52:49 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "master.hpp"
#include <vector>
#include <map>
#include <algorithm>

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

		void	parse_block(std::vector<std::string> context, std::string text);
		void	parse_server(const std::string &dir_content);
		void	parse_route(const std::string &dir_content);
		void	select_directive(const std::string &directive, const std::string &dir_content);
	public:
		Config(const std::string& path_to_config);
};

static bool				next_word_pos(const std::string& str, size_t &pos);
static size_t			word_len(const std::string& str, const size_t &pos);
void					show_error(const std::string& str, size_t pos);
size_t					end_directive(const std::string &text, const size_t &pos);
void					select_directive(const std::string &directive, const std::string &dir_content);

static void				parse_server();
static void				parse_route();
