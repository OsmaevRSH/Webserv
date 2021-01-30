#include "Cgi.h"

static char *content_type(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::iterator it = data.headers.getVariableHandlers().find("Content-type");

	if (it != data.headers.getVariableHandlers().end())
	{
		tmp = "CONTENT_TYPE = " + (*it).second;
		ret = strdup(tmp.c_str());
	}
	return ret;
}
static char *content_length(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::iterator it = data.headers.getVariableHandlers().find("Content-length");

	if (data.body.empty() == true)
		return ret;
	if (it != data.headers.getVariableHandlers().end())
	{
		tmp = "CONTENT_LENGTH = " + (*it).second;
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
	size_t		start_pos = data.headers.getUrl().find('?');

	if (start_pos == std::string::npos)
		return NULL;
	tmp = "QUERY_STRING = " + data.headers.getUrl().substr(start_pos);
	ret = strdup(tmp);
	return ret;
}
static char *auth_type(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::iterator it = data.headers.getVariableHandlers().find("Authorization");

	if (it != data.headers.getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = 0;
		for (; i < tmp.length() || isalnum(tmp[i]) == false; ++i){}
		tmp = tmp.substr(i);
		i = 0;
		for (; i < tmp.length() || isalnum(tmp[i]) == true; ++i){}
		tmp = "AUTH_TYPE = " + tmp.substr(i);
		ret = strdup(tmp.c_str());
	}
	return ret;
}
static char *remote_user(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::iterator it = data.headers.getVariableHandlers().find("Authorization");

	if (it != data.headers.getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = tmp.length() - 1;
		for (; i >= 0 || isalnum(tmp[i]) == false; --i){}
		for (; i >= 0 || isalnum(tmp[i]) == true; --i){}
		tmp = tmp.substr(i);
		tmp = "REMOTE_USER = " + tmp;
		ret = strdup(tmp.c_str());
	}
	return ret;
}
static char *remote_ident(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret 										= NULL;
	std::map<std::string, std::string>::iterator it = data.headers.getVariableHandlers().find("Authorization");

	if (it != data.headers.getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = tmp.length() - 1;
		for (; i >= 0 || isalnum(tmp[i]) == false; --i){}
		for (; i >= 0 || isalnum(tmp[i]) == true; --i){}
		tmp = tmp.substr(i);
		tmp = "REMOTE_IDENT = " + tmp + "." + (*it).second;
		ret = strdup(tmp.c_str());
	}
	return ret;
}

char		**get_meta_variables(const t_data_for_cgi &data)
{
	char **vars = (char**)malloc(sizeof(char*) * 17);

	vars[0] = strdup("SERVER_SOFTWARE = Webserver");
	vars[1] = strdup("GATEWAY_INTERFACE = CGI/1.1");
	vars[2] = strdup("SERVER_PROTOCOL = HTTP/1.1");
	vars[3] = strdup(("SERVER_NAME = " + data.server_ip).c_str());
	vars[4] = strdup(("SERVER_PORT = " + std::to_string(data.port)).c_str());
	vars[5] = strdup(("PATH_INFO = " + data.path_info).c_str());
	vars[6] = strdup(("PATH_TRANSLATED = " + data.path_translated).c_str());
	vars[8] = strdup(("REQUEST_METHOD = " + data.headers.getType()).c_str());
	vars[9] = strdup(("SCRIPT_NAME = " + data.script_name).c_str());
	vars[10] = strdup(("REMOTE_ADDR = " + data.client_ip).c_str());
	vars[11] = content_type(data);
	vars[12] = content_length(data);
	vars[13] = query_string(data);
	vars[14] = auth_type(data);
	vars[15] = remote_user(data);
	vars[16] = remote_ident(data);
	vars[17] = strdup(("REQUEST_URI = " + data.headers.getUrl()).c_str());
}