#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "OsDependency.hpp"

#include <string>
#include <vector>

// reference nginx index for more information.
// these variables are not taken directly from config file. they are processed to handle easier.

// ngx_http_headers_module
static const std::string	expires = "off";

// ngx_http_index_module
static const std::string	http_index = "index.html";

// ngx_http_autoindex_module
static const bool			autoindex = false;

// ngx_http_proxy_module
static const std::string	proxyPass = "";

// ngx_http_scgi_module
static const std::string	scgiPass = "";

// ngx_http_core_module
static const uint16_t		limitExcept = 0x1f;
static const std::string	root = "html";
static const std::string	alias = "";

// if clientMaxBodySize = 0, it's unlimited.
static const int64_t		clientMaxBodySize = 1000000;	// not necessary?

// static const std::vector<int16_t>	errorCode;
static const std::string	errorPagePath = "";

static const int32_t		listenIp = 0;
static const int32_t		listenPort = 8000;
static const int32_t		uriBufferSize = 8192;	// not necessary?

#endif
