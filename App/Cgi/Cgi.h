
#pragma once
#include "Parse_request_headers.hpp"
#include "master.hpp"

typedef struct				s_data_for_cgi
{
	std::string 			server_ip;
	std::string 			client_ip;
	std::string 			path_info; // Путь относительно директории сервера
	std::string 			path_translated; // Полный путь в файловой системе
	std::string 			script_name; // Виртуальный путь к скрипту (http путь)

	char					**env;
	int 					port;
	std::string				body;
	Parse_request_headers		*headers;
}							t_data_for_cgi;

class Cgi {
	private:
		int 			_pipe[2];
		std::string		_path_to_cgi;
		t_data_for_cgi	_data;
		char			*_args[3];
		char 			**_env;
		int 			tmp_fd;

		std::string 	_headers;
		std::string 	_body;
		long long		_content_length;
		int 			_status_code;
public:
		Cgi(const std::string &path_to_cgi, const t_data_for_cgi &data);
		~Cgi();
		void handleRequest();
		const char 	*getResponse();
		void		parse_cgi_response();

		int getStatusCode() const;
		const std::string &getHeaders() const;
		const std::string &getBody() const;
		long long getContentLength() const;
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



