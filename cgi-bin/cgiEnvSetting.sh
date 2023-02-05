#!/bin/bash

export REDIRECT_STATUS=404
export CONTENT_TYPE=
export CONTENT_LENGTH=
export SERVER_PROTOCOL=HTTP/1.1
export GATEWAY_INTERFACE=CGI/1.1
export REQUEST_METHOD=POST
export REQUEST_URI=/directory/youpla.bla
export PATH_INFO=/directory/youpla.bla
export PATH_TRANSLATED=/Users/soum/webserv/tester_dir/YoupiBanane/youpla.bla
# export SCRIPT_NAME=/cgi-bin/php-cgi
# export SCRIPT_FILENAME=/Users/soum/webserv/cgi-bin/php-cgi
export SCRIPT_NAME=/directory/youpla.bla
export SCRIPT_FILENAME=/Users/soum/webserv/tester_dir/YoupiBanane/youpla.bla
export QUERY_STRING=
export SERVER_NAME=
export SERVER_PORT=8080
export HTTP_HOST=localhost
export REMOTE_ADDR=
export REMOTE_HOST=
export REMOTE_IDENT=
export SERVER_SOFTWARE=webserv/1.0

./cgi_tester
