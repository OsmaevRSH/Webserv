#pragma once

#include "Search_by_configuration.hpp"

class PUT : public Search_by_configuration
{
	private:
		int _status_code;
		std::string &_head;
		std::string _body;
		std::string get_content_length();
		void get_PUT_status();
		void get_hendler();
	public:
		PUT(const Serv_conf &, const Parse_input_handler &, const MIME_ERROR &, std::string &, std::string &);
		void start_processing();
};
