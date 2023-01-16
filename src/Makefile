NAME		=	webserv

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -MMD
DEBUGFLAGS	=	-g -fsanitize=address
RM			=	rm -f

ifeq ($(shell uname), Linux)
SRC			= io/Epoll.cpp
else
SRC			= io/Kqueue.cpp
endif

SRC			+=	main.cpp\
				Logger.cpp\
				VirtualServer.cpp\
				Location.cpp\
\
				event/Client.cpp\
				event/Server.cpp\
				event/Cgi.cpp\
\
				parser/ConfigParser.cpp\
				parser/LocationParser.cpp\
				parser/ServerParser.cpp\
				parser/HttpRequestParser.cpp\
\
				tokenizer/HttpStreamTokenizer.cpp\
				tokenizer/FileTokenizer.cpp\
\
				exception/ConfigParserException.cpp\
				exception/HttpErrorHandler.cpp\
\
				http/Request.cpp\
				http/Response.cpp\
				http/HttpInfo.cpp\
\
				util/Util.cpp\

#				communicator/Communicator.cpp\
#				communicator/Request.cpp\
#				communicator/Response.cpp\
#\
#				socket/ClientSocket.cpp\
#				socket/ServerSocket.cpp\


OBJ			:=	$(SRC:%.cpp=%.o)
DEP			:=	$(OBJ:%.o=%.d)

INCL_PATH	=	-I./\
				-I../include\

STATUS		=	$(shell ls DEBUG.mode 2>/dev/null)
ifeq ($(STATUS), DEBUG.mode)
CXXFLAGS	+=	$(DEBUGFLAGS)
COMPILE_MODE=	DEBUG.mode
else
COMPILE_MODE=	RELEASE.mode
endif


.PHONY: all debug release clean fclean re

all: $(COMPILE_MODE)
	$(MAKE) $(NAME)

release: RELEASE.mode
	$(MAKE) all

debug: DEBUG.mode
	$(MAKE) all

RELEASE.mode:
	$(MAKE) fclean
	touch RELEASE.mode

DEBUG.mode:
	$(MAKE) fclean
	touch DEBUG.mode

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
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
