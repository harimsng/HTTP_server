# webserv
Single-threaded IO multiplexing HTTP web server written in C++.

# How to use

webserv uses ``kqueue()`` for IO multiplexing. MacOS or other supporting OS is mandatory.

GNU Make and C++ compiler supporting C++98 is mandatory.

webserv needs configuration file to initialize. configuration examples are at config directory.
configuration writing syntax is very similar to the nginx.

log level is adjustable by ``--log=[level]`` command line argument.
``ERROR``, ``WARNING``, ``INFO``, ``DEBUG``, ``VERBOSE`` are available(case sensitive).

# Details

we tried to implement server-side HTTP/1.1 defined in RFCs as much as possible, priotizing **MUST** and **SHALL** requirements like syntax BNF, ``Content-Length``, status code and etc.
