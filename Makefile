NAME		=	webserv

WEBSERV_ROOT	=	$(shell pwd)

.PHONY: all release debug clean fclean re

$(NAME): all

all:
	$(MAKE) -C src all RESOURCE_DIR=$(WEBSERV_ROOT)/
	cp src/$(NAME) .

release:
	$(MAKE) -C src release RESOURCE_DIR=$(WEBSERV_ROOT)/
	cp src/$(NAME) .

debug:
	$(MAKE) -C src debug RESOURCE_DIR=$(WEBSERV_ROOT)/
	cp src/$(NAME) .

clean:
	$(RM) $(NAME)
	$(MAKE) -C src clean

fclean: clean
	$(MAKE) -C src fclean

re: fclean
	$(MAKE) all
