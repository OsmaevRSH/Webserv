#pragma once

#include "master.hpp"

class MIME
{
	private:
		static std::map<std::string, std::string> _mime_types;
	public:
		MIME();
		std::string get_document_types(const std::string &) const;
};
