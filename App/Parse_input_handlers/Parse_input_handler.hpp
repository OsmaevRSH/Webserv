#pragma once

#include "master.hpp"

class Parse_input_handler
{
	private:
		std::string _type;
		std::string _url;
		std::string _protocol_type;
		std::map<std::string, std::string> _variable_handlers;
	public:
		explicit Parse_input_handler(const char *);
		Parse_input_handler &operator=(const Parse_input_handler &);
		~Parse_input_handler();

		static void parse_first_handler_string(std::string &, std::string &);
		const std::map<std::string, std::string> &getVariableHandlers() const;

		void setUrl(const std::string &url);

		const std::string &getType() const;
		const std::string &getUrl() const;
};