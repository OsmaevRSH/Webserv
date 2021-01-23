NAME = Webserv

EXE = Webserv

CXX = g++

CXXFLAGS = -W -Wextra -Werror --std=c++98

SRCS = App/Main/utils.cpp App/Parse_input_handlers/Parse_input_handler.cpp App/Main/main.cpp App/Server/Server.cpp \
	App/Metods/Autoindex_generate.cpp App/Server/Server_sys_function.cpp App/Metods/Select_metod.cpp \
	App/MIME&ERROR_CODE/MIME_ERROR.cpp App/Metods/GET/GET.cpp App/Metods/HEAD/HEAD.cpp \
	App/Metods/Search_by_configuration/Search_by_configuration.cpp Config/Srcs/Config_parser.cpp \
	App/Metods/Search_by_configuration/Header.cpp

HEADERS_DIR = App/Main/master.hpp App/MIME&ERROR_CODE/MIME_ERROR.hpp App/Parse_input_handlers/Parse_input_handlers.hpp \
	App/Server/Server.hpp App/Metods/Search_by_configuration/Search_by_configuration.hpp App/Metods/GET/GET.hpp \
	App/Metods/HEAD/HEAD.hpp App/Metods/PUT/PUT.hpp Config/Headers/Config.hpp

#HEADERS_DIR = App/Main/ App/MIME&ERROR_CODE/ App/Parse_input_handlers/ \
#	App/Server/ App/Metods/Search_by_configuration/ App/Metods/GET/ \
#	App/Metods/HEAD/ App/Metods/PUT/ Config/Headers/

OBJS = $(SRCS:.cpp=.o)

all: $(EXE)

$(EXE): $(OBJS)
		$(CXX) $(CXXFLAGS) -I$(HEADERS_DIR) $(OBJS) -o $(NAME)

start: $(NAME)
		$(MAKE) clean
		./Webserv

clean:
		$(RM) $(OBJS)

fclean:
		$(RM) $(OBJS) $(NAME)

re: fclean all