#pragma once

#include "master.hpp"

class Parse_request_headers
{
	private:
		std::string _type;
		std::string _url;
		std::string _protocol_type;
		std::string _server_ip;
		std::string _client_ip;
		bool		_error;
		std::map<std::string, std::string> _variable_handlers;
	public:
		explicit Parse_request_headers(const char *, std::string &, std::string &);
		Parse_request_headers &operator=(const Parse_request_headers &);
		~Parse_request_headers();

		static void parse_first_handler_string(std::string &, std::string &);
		const std::map<std::string, std::string> &getVariableHandlers() const;

		void setUrl(const std::string &url);
		const std::string &getType() const;
		const std::string &getUrl() const;
		const std::string &getServerIp() const;
		const std::string &getClientIp() const;
		bool isError() const;
		void headersIsOk();
};