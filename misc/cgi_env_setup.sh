# conditional MUST set for authentication
# AUTH_TYPE			= "" | auth-scheme
# ...
AUTH_TYPE=""

# MUST set, number of octets of message content after any transfer-codings or content-codings.
# CONTENT_LENGTH	= "" | 1*digit
CONTENT_LENGTH=""

# conditional MUST set if HTTP Content-Type fields is present in the request header.
# CONTENT_TYPE		= "" | media-type
# ...
CONTENT_TYPE=""

# MUST set, CGI/(major version).(minor version)
# GATEWAY_INTERFACE	= "CGI" "/" 1*digit "." 1*digit
GATEWAY_INTERFACE="CGI/1.1"

# conditional MUSTs
# part of URI after script's path.
# PATH_INFO			= "" | ( "/" path )
PATH_INFO=""

# conditional MUSTs
# actual path of resource.
# PATH_TRANSLATED	= *<any character>
PATH_TRANSLATED=""

# MUST set, empty string if uri doesn't include query-string
# QUERY_STRING		= query-string
# query-string		= *uric
# uric				= reserved | unreserved | escaped
QUERY_STRING=""

# MUST set
# network address of the client
# REMOTE_ADDR		= hostnumber
# hostnumber		= ipv4-address | ipv6-address
REMOTE_ADDR=127.0.0.1

# SHOULD set
# domain name of the client
REMOTE_HOST=""

# MAY be used
# identity information
# REMOTE_IDENT		= *TEXT

# conditional MUST set if client required HTTP Authentication
# REMOTE_USER		= *TEXT

# MUST set
# REQUEST_METHOD	= method
# method			= "GET" | "POST" | "HEAD" | extension-method
# extension-method	= "PUT" | "DELETE" | token
REQUEST_METHOD="POST"

# MUST set
# part of URI path which could identify the CGI script
# SCRIPT_NAME		= "" | ( "/" path )
# if uri = /cgi-bin/cgi.php/foo/bar, /cgi-bin/cgi.php
SCRIPT_NAME=""

# MUST set
# SERVER_NAME		= server-name
# server-name		= hostname | ipv4-address | ( "[" ipv6-address "]" )
SERVER_NAME=""

# MUST set
# TCP/IP port number on which request is received from the client
SERVER_PORT=""

# MUST set
# SERVER_PROTOCOL	= HTTP-Version | "INCLUDED" | extension-version
# HTTP-Version		= "HTTP" "/" 1*digit "." 1*digit
SERVER_PROTOCOL="HTTP/1.1"

# MUST set
# name and version of the server making the CGI request
# SHOULD same as server description reported to the client, if any.
# SERVER_SOFTWARE	= 1*( product | comment )
# product			= token [ "/" product-version ]
# product-version	= token
SERVER_SOFTWARE="webserv"
