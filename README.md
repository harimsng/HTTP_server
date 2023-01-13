# webserv
Single-threaded Io multiplexing web server written in C++.

it requires configuration file with extension name ".conf".
rules(syntax, server name resolution) are similar to nginx.

referenced rfc9110, rfc9112 to implement HTTP/1.1 communication, but some parts of these rfcs are actually implemented.
