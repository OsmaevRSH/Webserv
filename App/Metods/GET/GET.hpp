#pragma once

#include "Search_by_configuration.hpp"

class GET : public Search_by_configuration
{
	protected:
		std::string &_body;
		std::string &_hendler;

		void get_hendler();
		virtual void get_page();
		virtual std::string get_content_length();
	public:
		GET(const Serv_conf &, const Parse_input_handler &, const MIME_ERROR &, std::string &, std::string &);
		void start_processing();
};
