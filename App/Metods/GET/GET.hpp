#pragma once

#include "Path.hpp"

class GET : public Path
{
	private:
	public:
		GET(const Serv_conf &, const Parse_input_handler &, const MIME_ERROR &);
		std::string get_page();
		std::string get_hendler(const std::string &);
};
