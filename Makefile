NAME		=	webserv


CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
DEBUGFLAGS	=	-g -fsanitize=address
RM			=	rm -f


SRC			=	main.cpp\
				ServerManager.cpp\
				Server.cpp\
				Location.cpp\
\
				parser/ConfigParser.cpp\
				parser/LocationParser.cpp\
				parser/ServerParser.cpp\
				parser/HttpRequestParser.cpp\
\
				tokenizer/ATokenizer.cpp\
				tokenizer/SocketTokenizer.cpp\
				tokenizer/FileTokenizer.cpp\
\
				exception/ConfigParserException.cpp\
				exception/HttpRequestParserException.cpp\
\
				socket/ClientSocket.cpp\
				socket/ServerSocket.cpp\
\
				communicator/Communicator.cpp\
				communicator/Request.cpp\
				communicator/Response.cpp\
\
				cgi/cgi.cpp\


OBJ			=	$(SRC:%.cpp=%.o)

INCL_PATH	=	-I./\
				-Iparser/\
				-Itokenizer/\
				-Isocket/\
				-Icommunicator/\
				-Icgi/

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
