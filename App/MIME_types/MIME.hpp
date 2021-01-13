#pragma once

#include "../Main/master.hpp"

class MIME
{
	private:
		std::map<std::string, std::string> _mime_types;
	public:
		MIME();
		std::string get_document_types(const std::string &) ;
};
