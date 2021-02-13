#pragma once

#include "Search_by_configuration.hpp"

class GET : public Search_by_configuration
{
	protected:
		std::string &_body;
//		char *_body;
		std::string &_head;

		void get_hendler();
		virtual void get_page();
		virtual std::string get_content_length();
	public:
		GET(const Serv_conf &, const Parse_request_headers &, const MIME_ERROR &, std::string &, std::string &, std::list<Client>::iterator &);
		void start_processing();
};
