/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 19:18:58 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/20 20:21:52 by jeldora          ###   ########.fr       */
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





/*
1) Нашли слово? посмотрели, есть ли такое в нашем контесксте.
2) В зависимости от того, блочная это директива или простая, ищем завершающий символ.
3) Бросаем в соответствующую функцию начало и коней строки с аргументами директивы. Обрабатываем.
*/

Config::				Config(const std::string& path_to_config)
{
	std::string text	= get_page_text(path_to_config);
	size_t		pos		= 0;

	std::vector<std::string> main_context;
	main_context.push_back("index");
	main_context.push_back("max_body_size");
	main_context.push_back("root");
	main_context.push_back("autoindex");
	main_context.push_back("server");
	main_context.push_back("error_page");

	while (next_word_pos(text, pos))
	{
		std::string word = text.substr(pos, word_len(text, pos));

		if (std::find(main_context.begin(), main_context.end(), word) == main_context.end())
		{
			show_error(text, pos);
			exit(1);
		}

		pos += word_len(text, pos);
		size_t end_pos = end_directive(text, pos);
		// позицию, конец и имя директивы я посылаю в каждую функцию. И там уже происходит парсинг.
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