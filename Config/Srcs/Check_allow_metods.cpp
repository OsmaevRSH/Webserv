#include "Config.hpp"

bool Config::check_allow_metods(const ConfigHandler::t_params &param, Input_handlers &handlers)
{
	if (param.allow_methods.empty())
		return false;
	else
	{
		for (int i = 0; i < static_cast<int>(param.allow_methods.size()); ++i)
		{
			if (param.allow_methods[i] == handlers.getType())
				return false;
		}
		return true;
	}
}