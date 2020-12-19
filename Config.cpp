/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:18:58 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/20 02:37:48 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
**	Как будет устроен парсер? Какие есть крайние случаи?
**	Неправильное слово, 
**
**	Во время парсинга будет переменная, которая будет отслеживать,
**	на какой строке находиться парсер. Если нейдет ошибку, вернет номер строки. От места с ошибкой,
**	мы запускаем цикл, который будет в обратном направлении искать '\n' и считать сколько их. Кол-во '\n'
**	+ 1 это номер строки. Еще можно узнать номер стркоки, посчитав кол-во символов от первого '\n', если
**	он есть.

**	Будет несколько модулей парсеров, которые будут вызываться один из другого

**	Итак, что мы ждем в мейне?
**	MAIN: server | error_page | 					[ index | max_body_size | root | autoindex ]
**	SERVER: ip | port | server_name | route | 		[ index | max_body_size | root | autoindex ]
**	ROUTE: allow | route | 							[ index | max_body_size | root | autoindex ]

**	Значит, мы находим слово. Мы должны узнать, существует ли оно в этом контексте. 
**	Если нет, возвращаем ошибку с номером строки (и так будет во всех дальнейших негативных случаях).
**	
**	Окей. Получили слово. Мы должны что-то ждать от него. Например, если это блочная директива, то мы
**	ждем фигурные скобки, и парсим все, что будет внутри них. Если это простая директива, то мы
**	знаем, что она завершится точкой с запятой, соотетственно, обрабатываем все, что есть до нее.
*/


/* Возвращает true, если находит новое слово, false если конец строки */
static bool				next_word_pos(const std::string& str, size_t &pos)
{
	while (pos < str.length() && str[pos] == ' ')
		pos++;
	if (pos == str.length())
		return false;
	return true;
}

static size_t			word_len(const std::string& str, const size_t &pos)
{
	size_t length = 0;

	while (pos + length < str.length() && str[pos + length] != ' ')
		length++;
	return length;
}

/*
static void				parse_server()
{

}

static void				parse_route()
{

}
*/

Config::				Config(const std::string& path_to_config)
{
	std::string text	= get_page_text(path_to_config);
	size_t		pos		= 0;

	std::vector<std::string> main_context;
	std::vector<std::string> server_context;
	std::vector<std::string> route_context;

	main_context.push_back("index");
	main_context.push_back("max_body_size");
	main_context.push_back("root");
	main_context.push_back("autoindex");
	main_context.push_back("server");
	main_context.push_back("error_page");


	server_context.push_back("index");
	server_context.push_back("max_body_size");
	server_context.push_back("root");
	server_context.push_back("autoindex");
	server_context.push_back("ip");
	server_context.push_back("port");

	while (next_word_pos(text, pos))
	{
		/*
		** Получили начало слова, получили его длину. Что дальше?
		** Вычленяем это слово, и сравниваем с возможными. Получается, для каждого
		** возможного слова, должна быть своя функция, которая обработает это слово.
		*/
	}
}

Config::_Everywhere::	_Everywhere()
{
	root = "";
	index = "";
	max_body_size = 0;
	autoindex = false;
}

Config::_Route::		_Route()
{
}

Config::_Server::		_Server()
{
	port = 0;
	ip = "";
	server_name = "";
}