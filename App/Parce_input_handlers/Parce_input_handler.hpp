#pragma once

#include "master.hpp"

class Parce_input_handler
{
	private:
		std::string _type;
		std::string _url;
		std::string _protocol_type;
		void Map_init();
		std::map<std::string, std::string> _variable_handlers;
		t_headers _handlers;
		std::map<std::string, std::string &> _config_list;
	public:
		explicit Parce_input_handler(const char *);
		Parce_input_handler(const Parce_input_handler &);
		Parce_input_handler &operator=(const Parce_input_handler &);
		~Parce_input_handler();

		t_headers &getHandlers();
		static void parce_first_handler_string(std::string &, std::string &);
		const std::string &getType() const;
		const std::string &getUrl() const;
		const std::string &getProtocolType() const;
		void setUrl(const std::string &url);
		const std::map<std::string, std::string> &getVariableHandlers() const;
};