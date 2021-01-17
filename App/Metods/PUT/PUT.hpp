#pragma once

#include "Search_by_configuration.hpp"

class PUT : public Search_by_configuration
{
	private:
		int _status_code;
		std::string &_body;
//		std::string get_content_length() {};
		std::string get_content_location();
		std::string get_PUT_status();
		void get_hendler();
	public:
		PUT(const Serv_conf &, const Parse_input_handler &, const MIME_ERROR &, std::string &);
		void start_processing();
};
