#include "GET.hpp"

GET::GET(const Serv_conf &serv, const Parse_input_handler &handler)
		: Path(serv, handler) {}
