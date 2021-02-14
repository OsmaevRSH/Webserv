#include "GetMimeTypeAndErrorPageCode.hpp"

MIME_ERROR::MIME_ERROR()
{
	size_t it;

	std::ifstream mime_file("../App/MimeType_And_ErrorPageCode/mime_types");
	std::ifstream error_file("../App/MimeType_And_ErrorPageCode/error_types");
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

std::string MIME_ERROR::get_error_page(int errorCode)
{
	std::stringstream ErrorPage;
	ErrorPage << "<!DOCTYPE html>\n"
	             "<html lang=en>\n"
	             "<meta charset=utf-8>\n"
	             "<meta name=viewport content=\"initial-scale=1, minimum-scale=1, width=device-width\">\n"
	             "<title>Error " << errorCode << "</title>\n"
	                                             "<style>\n"
	                                             "    *{margin:0;padding:0}html,code{font:15px/22px arial,sans-serif}html{background:#fff;color:#222;padding:15px}body{margin:7% auto 0;max-width:390px;min-height:180px;padding:30px 0 15px}* > body{background:url(//www.google.com/images/errors/robot.png) 100% 5px no-repeat;padding-right:205px}p{margin:11px 0 22px;overflow:hidden}ins{color:#777;text-decoration:none}a img{border:0}@media screen and (max-width:772px){body{background:none;margin-top:0;max-width:none;padding-right:0}}#logo{background:url(//www.google.com/images/branding/googlelogo/1x/googlelogo_color_150x54dp.png) no-repeat;margin-left:-5px}@media only screen and (min-resolution:192dpi){#logo{background:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) no-repeat 0% 0%/100% 100%;-moz-border-image:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) 0}}@media only screen and (-webkit-min-device-pixel-ratio:2){#logo{background:url(//www.google.com/images/branding/googlelogo/2x/googlelogo_color_150x54dp.png) no-repeat;-webkit-background-size:100% 100%}}#logo{display:inline-block;height:54px;width:150px}\n"
	                                             "</style>\n"
	                                             "<a href=//www.google.com/><span id=logo aria-label=Google></span></a>\n"
	                                             "<p><b>" << errorCode << ".</b> <ins>That’s an error.</ins>\n"
	                                                                      "<p>The requested URL <code></code> was not found on this server.  <ins>That’s all we know.</ins>";
	return ErrorPage.str();
}

