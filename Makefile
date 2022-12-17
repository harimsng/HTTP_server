NAME		=	webserv

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -MMD
DEBUGFLAGS	=	-g -fsanitize=address
RM			=	rm -f

SRC			=	main.cpp\
				ServerManager.cpp\
				Server.cpp\
				Location.cpp\
				Logger.cpp\
\
				parser/config_parser/ConfigParser.cpp\
				parser/config_parser/LocationParser.cpp\
				parser/config_parser/ServerParser.cpp\
				parser/HttpRequestParser.cpp\
\
				tokenizer/ATokenizer.cpp\
				tokenizer/SocketTokenizer.cpp\
				tokenizer/FileTokenizer.cpp\
\
				socket/ClientSocket.cpp\
				socket/ServerSocket.cpp\
\
				communicator/Communicator.cpp\
				communicator/Request.cpp\
				communicator/Response.cpp\
\
				cgi/Cgi.cpp\


OBJ			:=	$(SRC:%.cpp=%.o)
DEP			:=	$(OBJ:%.o=%.d)

INCL_PATH	=	-I./\

ifeq ($(DEBUG_MODE), 1)
CXXFLAGS	+=	$(DEBUGFLAGS)
COMPILE_MODE=	DEBUG.mode
else
COMPILE_MODE=	RELEASE.mode
endif


.PHONY: all debug clean fclean re

all: $(COMPILE_MODE)
	$(MAKE) $(NAME)

RELEASE.mode:
	$(MAKE) fclean
	touch RELEASE.mode

DEBUG.mode:
	$(MAKE) fclean
	touch DEBUG.mode

debug:
	$(MAKE) DEBUG_MODE=1 all

clean:
	$(RM) $(OBJ)
	$(RM) RELEASE.mode DEBUG.mode

fclean: clean
	$(RM) $(NAME)

re: fclean
	$(MAKE) all

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCL_PATH)

$(OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCL_PATH)

-include $(DEP)
