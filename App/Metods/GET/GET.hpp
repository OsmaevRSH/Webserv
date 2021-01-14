#pragma once

#include "Path.hpp"

class GET : public Path
{
	protected:
		std::string &_body;
		std::string &_hendler;

		void get_page();
		void get_hendler();
	public:
		GET(const Serv_conf &, const Parse_input_handler &, const MIME_ERROR &, std::string &, std::string &);
		void get_start();
};
