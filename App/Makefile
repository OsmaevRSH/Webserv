NAME = Webserv

CXX = clang++

#CPPFLAGS = -W -Wextra -Werror --std=c++98

SRCS = Server.cpp main.cpp get_page_text.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
		$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME)

start: $(NAME)
		$(MAKE) clean
		./Webserv

clean:
		$(RM) $(OBJS)

fclean:
		$(RM) $(OBJS) $(NAME)

re: fclean all