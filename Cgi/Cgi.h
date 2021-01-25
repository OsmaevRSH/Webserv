
#pragma once
#include "../App/Main/master.hpp"
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include "../App/Parse_input_handlers/Parse_input_handler.hpp"

typedef Parse_input_handler data;

typedef struct				s_data_for_cgi
{
	std::string				body;
	Parse_input_handler		headers;
}							t_data_for_cgi;

class Cgi {
	private:
		std::string _path_to_cgi;
		char		**_args;
		char		**_env;
	public:
		Cgi(const std::string &path_to_cgi, const t_data_for_cgi &data);
		std::string &handleRequest();

};

/*
 * Нужно: тело сообщения
 *
 * Нужно сформировать заголовки, т.е. нужно послать все данные, из которых нужно будет достать информацию для заголовков.
 * Открыть пайп.
 * Подменить фд1 и записать в него тело сообщения
 * Форк, дапаем 0, дапаем 1
 * Отдать все в виде строки клиенту.
 *
 */



