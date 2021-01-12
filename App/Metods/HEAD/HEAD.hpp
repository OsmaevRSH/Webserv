#pragma once

#include "master.hpp"
#include "MIME.hpp"
#include "../Search_path(BASE)/Path_to_file.hpp"

class HEAD : public Path_to_file
{
	private:
		static std::string get_date_handler();
	public:
};
