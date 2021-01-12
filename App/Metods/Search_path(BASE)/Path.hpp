#pragma once

bool check_slash(Parse_input_handler &handlers);
bool search_folder(ConfigHandler::t_params &params, Parse_input_handler &handlers);
bool search_file(ConfigHandler::t_params &params, Parse_input_handler &handlers);

void update_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_location &location);
void location_sort(std::vector <ConfigParser::t_location> &locations);

template<class T>
ConfigParser::t_location *check_path_with_complete_coincidence(T &param, Parse_input_handler &handlers);
template<class T>
ConfigParser::t_location *check_simple_location(T &param, Parse_input_handler &handlers);
