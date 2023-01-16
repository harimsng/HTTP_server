NAME	=	webserv

.PHONY: all release debug clean fclean re

$(NAME): all

all:
	$(MAKE) -C src all
	cp src/$(NAME) .

release:
	$(MAKE) -C src release
	cp src/$(NAME) .

debug:
	$(MAKE) -C src debug
	cp src/$(NAME) .

clean:
	$(MAKE) -C src clean

fclean: clean
	$(MAKE) -C src fclean

re: fclean
	$(MAKE) -C src all
