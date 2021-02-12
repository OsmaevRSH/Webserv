#pragma once

#include "master.hpp"
#include "GetMimeTypeAndErrorPageCode.hpp"
#include "GET.hpp"

class HEAD : public GET
{
	private:
		std::string _tmp_body;

		void get_page();
		std::string get_content_length();
	public:
		HEAD(const Serv_conf &, const Parse_request_headers &, const MIME_ERROR &, std::string &, std::string &, std::list<Client>::iterator &);
};
