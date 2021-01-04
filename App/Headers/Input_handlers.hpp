#pragma once

#include "shared.hpp"

class Input_handlers
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
		explicit Input_handlers(const char *);
		Input_handlers(const Input_handlers &);
		Input_handlers &operator=(const Input_handlers &);
		~Input_handlers();

		t_headers &getHandlers();
		static void parce_first_handler_string(std::string &, std::string &);
		const std::string &getType() const;
		const std::string &getUrl() const;
		const std::string &getProtocolType() const;
		void setUrl(const std::string &url);

		void output() const
		{
			std::cout << "\nType: " << _type << "\nUrl: " << _url
			   << "\nProtocol_type: " << _protocol_type << std::endl;
			for (std::map<std::string, std::string>::const_iterator it = _variable_handlers.cbegin(); it != _variable_handlers.cend(); ++it)
			{
				std::cout << it->first << ": " << it->second << std::endl;
			}
			std::cout << std::endl;
		}
};