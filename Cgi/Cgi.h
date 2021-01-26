
#pragma once
#include "../App/Main/master.hpp"
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include "../App/Parse_input_handlers/Parse_input_handler.hpp"
#include <stdio.h>
#include <stdlib.h>

typedef struct				s_data_for_cgi
{
	std::string 			server_ip;
	int 					port;
	std::string 			path_info; // Путь относительно директории сервера
	std::string 			path_translated; // Полный путь в файловой системе
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

char **get_meta_variables(const t_data_for_cgi &data);
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



