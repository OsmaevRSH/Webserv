#include "Input_handlers.hpp"

void Method_selector(Input_handlers &inputHandlers, std::string &handler, std::string &body)
{
	if (inputHandlers.getType() == "GET")
	{}
	else if (inputHandlers.getType() == "HEAD")
	{}
	else if (inputHandlers.getType() == "PUT")
	{}
	else if (inputHandlers.getType() == "POST")
	{}
	else if (inputHandlers.getType() == "TRACE")
	{}
	else if (inputHandlers.getType() == "DELETE")
	{}
	else if (inputHandlers.getType() == "CONNECT")
	{}
	else if (inputHandlers.getType() == "OPTIONS")
	{}
	else
	{
		std::cerr << "Error: Invalid method\n";
		exit(EXIT_FAILURE);
	}
}