#include "Cgi.h"

static char *content_type(const t_data_for_cgi &data)
{
	std::string tmp;
	char *ret = NULL;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Content-Type");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = "CONTENT_TYPE=" + (*it).second;
		ret = strdup(tmp.c_str());
	}
	return ret;
}

static char *query_string(const t_data_for_cgi &data)
{
	std::string tmp;
	size_t start_pos = data.headers->getUrl().find('?');

	if (start_pos == std::string::npos)
		return strdup("QUERY_STRING=");
	tmp = "QUERY_STRING=" + data.headers->getUrl().substr(start_pos);
	return strdup(tmp.c_str());;
}
static char *auth_type(const t_data_for_cgi &data)
{
	std::string tmp;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Authorization");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = 0;
		for (; i < tmp.length() || isalnum(tmp[i]) == false; ++i)
		{
		}
		tmp = tmp.substr(i);
		i = 0;
		for (; i < tmp.length() || isalnum(tmp[i]) == true; ++i)
		{
		}
		tmp = "AUTH_TYPE=" + tmp.substr(i);
		return strdup(tmp.c_str());
	}
	return strdup("AUTH_TYPE=");
}
static char *remote_user(const t_data_for_cgi &data)
{
	std::string tmp;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Authorization");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = tmp.length() - 1;
		for (; i >= 0 || isalnum(tmp[i]) == false; --i)
		{
		}
		for (; i >= 0 || isalnum(tmp[i]) == true; --i)
		{
		}
		tmp = tmp.substr(i);
		tmp = "REMOTE_USER=" + tmp;
		return strdup(tmp.c_str());
	}
	return strdup("REMOTE_USER=");
}
static char *remote_ident(const t_data_for_cgi &data)
{
	std::string tmp;
	std::map<std::string, std::string>::const_iterator it = data.headers->getVariableHandlers().find("Authorization");

	if (it != data.headers->getVariableHandlers().end())
	{
		tmp = (*it).second;
		int i = tmp.length() - 1;
		for (; i >= 0 || isalnum(tmp[i]) == false; --i)
		{
		}
		for (; i >= 0 || isalnum(tmp[i]) == true; --i)
		{
		}
		tmp = tmp.substr(i);
		tmp = "REMOTE_IDENT=" + tmp + "." + (*it).second;
		return strdup(("REMOTE_IDENT=" + tmp + "." + (*it).second).c_str());
	}
	return strdup("REMOTE_IDENT=");
}

std::string get_meta_var(const std::string &str)
{
	std::string::const_iterator it = str.begin();
	std::string output = "HTTP_";
	for (; it != str.end(); ++it)
	{
		if (std::isalpha(*it))
			output += static_cast<char>(std::toupper(*it));
		if (*it == '-')
			output += '_';
	}
	return output;
}

char **get_meta_variables(const t_data_for_cgi &data)
{
	std::map<std::string, std::string>::const_iterator it;
	it = data.headers->getVariableHandlers().begin();
	int i = 0;
	char **vars = new char *[40];

	vars[i++] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	vars[i++] = strdup("PATH_INFO=/");
	vars[i++] = strdup(("REQUEST_METHOD=" + data.headers->getType()).c_str());
	vars[i++] = strdup(("SERVER_NAME=" + data.server_ip).c_str());
	vars[i++] = strdup(("SERVER_PORT=" + std::to_string(data.port)).c_str());
	vars[i++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	vars[i++] = strdup("SERVER_SOFTWARE=Webserver/1.0");
	vars[i++] = strdup("SCRIPT_NAME=");
	vars[i++] = query_string(data);
	vars[i++] = strdup(("REMOTE_ADDR=" + data.client_ip).c_str());
	vars[i++] = strdup(("PATH_TRANSLATED=" + data.path_translated).c_str());
	vars[i++] = auth_type(data);
	vars[i++] = remote_ident(data);
	vars[i++] = remote_user(data);
	for (;it != data.headers->getVariableHandlers().end(); ++it)
		vars[i++] = strdup((get_meta_var(it->first) + "=" + it->second).c_str());
	vars[i] = nullptr;
	return vars;
}