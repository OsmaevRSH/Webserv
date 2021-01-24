
#pragma once
#include "../App/Main/master.hpp"
#include <sys/wait.h>

class Cgi {
	private:
		std::string _path_to_sgi;
		char		**_args;
		char		**_env;
	public:
		Cgi(const std::string &path_to_cgi, char **args, char **env);
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



