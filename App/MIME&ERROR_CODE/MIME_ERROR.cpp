#include "MIME_ERROR.hpp"

MIME_ERROR::MIME_ERROR()
{
	size_t it;

	std::ifstream mime_file("./App/MIME&ERROR_CODE/mime_types");
	std::ifstream error_file("./App/MIME&ERROR_CODE/error_types");
	if (!mime_file.is_open() || !error_file.is_open())
	{
		std::cout << "Error: Файл не может быть открыт!\n";
		exit(EXIT_FAILURE);
	}
	for (std::string line; getline(mime_file, line);)
	{
		it = line.find(' ');
		_mime_types.insert(std::pair<std::string, std::string>(line.substr(0, it), line.substr(it + 1)));
	}
	mime_file.close();
	for (std::string line; getline(error_file, line);)
	{
		it = line.find(' ');
		_error_types.insert(std::pair<std::string, std::string>(line.substr(0, it), line.substr(it + 1)));
	}
	error_file.close();
}

std::string MIME_ERROR::get_document_types(const std::string &path)
{
	size_t pos;

	if ((pos = path.find_last_of('.')) == std::string::npos)
		return "text/plain";
	if (_mime_types.find(path.substr(pos)) == _mime_types.end())
		return "text/plain";
	return _mime_types[path.substr(pos)];
}

std::string MIME_ERROR::get_error(int error_code)
{
	return _error_types[std::to_string(error_code)];
}

