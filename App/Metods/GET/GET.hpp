#pragma once

#include "../Path/Path.hpp"

class GET : public Path
{
	public:
		GET(const Serv_conf &, const Parse_input_handler &);
};
