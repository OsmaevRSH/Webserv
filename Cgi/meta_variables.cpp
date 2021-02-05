#include "Cgi.h"

static char *content_type(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Content-type");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = "CONTENT_TYPE=" + (*it).second;
		ret = strdup(tmp.c_str());
	}
	return ret;
}
static char *content_length(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Content-length");

	if (data.body.empty() == true)
		return ret;
	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = "CONTENT_LENGTH=" + (*it).second;
		ret = strdup(tmp.c_str());
	}
	else
	{
		size_t len = data.body.length();
		ret = strdup(std::to_string(len).c_str());
	}
	return ret;
}
static char *query_string(const t_data_for_cgi &data)
{
	std::string tmp;
	char		*ret = NULL;
	size_t		start_pos = data.headers->getUrl().find('?');

	if (start_pos == std::string::npos)
		return NULL;
	tmp = "QUERY_STRING=" + data.headers->getUrl().substr(start_pos);
	ret = strdup(tmp.c_str());
	return ret;
}
static char *auth_type(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Authorization");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = 0;
		for (; i < tmp.length() || isalnum(tmp[i]) == false; ++i){}
		tmp = tmp.substr(i);
		i = 0;
		for (; i < tmp.length() || isalnum(tmp[i]) == true; ++i){}
		tmp = "AUTH_TYPE=" + tmp.substr(i);
		ret = strdup(tmp.c_str());
	}
	return ret;
}
static char *remote_user(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Authorization");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = tmp.length() - 1;
		for (; i >= 0 || isalnum(tmp[i]) == false; --i){}
		for (; i >= 0 || isalnum(tmp[i]) == true; --i){}
		tmp = tmp.substr(i);
		tmp = "REMOTE_USER=" + tmp;
		ret = strdup(tmp.c_str());
	}
	return ret;
}
static char *remote_ident(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Authorization");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = tmp.length() - 1;
		for (; i >= 0 || isalnum(tmp[i]) == false; --i){}
		for (; i >= 0 || isalnum(tmp[i]) == true; --i){}
		tmp = tmp.substr(i);
		tmp = "REMOTE_IDENT=" + tmp + "." + (*it).second;
		ret = strdup(tmp.c_str());
	}
	return ret;
}

const int len_arr(char **env)
{
	char **tmp = env;
	int len = 0;

	for (int i = 0; env[i]; ++i)
		len++;
	return len;
}

char		**get_meta_variables(const t_data_for_cgi &data)
{
//	vars[2] = strdup("SERVER_PROTOCOL = HTTP/1.1");
//	vars[8] = strdup(("REQUEST_METHOD = " + data.headers->getType()).c_str());

	int darray_len = len_arr(data.env);

	int i = 0;
	char **vars = (char**)calloc(sizeof(char*), (18 + darray_len));

//	for (i = 0; data.env[i]; ++i)
//		vars[i] = strdup(data.env[i]);

	vars[i + 0] = strdup("CONTENT_LENGTH=100000000");
	vars[i + 1] = strdup("CONTENT_TYPE=text/html");
	vars[i + 2] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	vars[i + 3] = strdup("PATH_INFO=/");
//	/Users/ltheresi/CLionProjects/Webserv/Tester/YoupiBanane/youpi.bla
	vars[i + 4] = strdup("QUERY_STRING=");
	vars[i + 5] = strdup("REQUEST_METHOD=POST");
	vars[i + 6] = strdup("SERVER_NAME=127.0.0.1");
	vars[i + 7] = strdup("SERVER_PORT=1234");
	vars[i + 8] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	vars[i + 9] = strdup("SERVER_SOFTWARE=Webserver");
	vars[i + 10] = strdup("SCRIPT_NAME="/* + data.script_name.substr(1)*//*.c_str()*/);
	//vars[i + 11] = strdup("REQUEST_URI=/test/youpi.bla");
//	vars[i + 11] = strdup("PATH_TRANSLATED=/Users/ltheresi/CLionProjects/Webserv/Tester/YoupiBanane");


//	vars[i + 3] = strdup(("SERVER_NAME=" + data.server_ip).c_str());
//	vars[i + 4] = strdup(("SERVER_PORT=" + std::to_string(data.port)).c_str());
//	vars[i + 6] = strdup(("REQUEST_METHOD=" + data.headers->getType()).c_str());
//	vars[i + 8] = strdup(("REMOTE_ADDR=" + data.client_ip).c_str());
//	vars[i + 9] = content_type(data);
//	vars[i + 10] = content_length(data);
//	vars[i+ 6] = strdup(("PATH_TRANSLATED=" + data.path_translated)
//	vars[i+ 7] = strdup(("REQUEST_URI=" + data.headers->getUrl()).c_str());.c_str());
//	vars[i+ 13] = query_string(data);
//	vars[i+ 14] = auth_type(data);
//	vars[i+ 15] = remote_user(data);
//	vars[i+ 16] = remote_ident(data);

	return vars;
}