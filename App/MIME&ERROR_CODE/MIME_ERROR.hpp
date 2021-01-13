#pragma once

#include "master.hpp"

class MIME_ERROR
{
	private:
		std::map<std::string, std::string> _mime_types;
		std::map<std::string, std::string> _error_types;
	public:
		MIME_ERROR();
		std::string get_document_types(const std::string &);
		std::string get_error(int);
};
