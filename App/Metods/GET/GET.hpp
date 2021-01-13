#pragma once

#include "Path.hpp"

class GET : public Path
{
	public:
		GET(const Serv_conf &, const Parse_input_handler &);
		std::string get_page();
};
