#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "OsDependency.hpp"

#include <string>
#include <vector>

static const std::string	expires = "off";

static const std::string	http_index = "index.html";

static const bool			autoindex = false;

static const std::string	cgiPass = "";

static const uint16_t		limitExcept = 0x1f;
static const std::string	root = "html";
static const std::string	alias = "";

static const int64_t		clientMaxBodySize = 10000000;

static const std::string	errorPagePath = "";

static const int32_t		listenIp = 0;
static const int32_t		listenPort = 8000;
static const int32_t		uriBufferSize = 8192;

#endif
