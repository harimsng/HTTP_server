#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "OsDependency.hpp"

#include <string>
#include <vector>

static const std::string	defaultExpires = "off";

static const std::string	defaultHttpIndex = "index.html";

static const bool			defaultAutoindex = false;

static const std::string	defaultCgiPass = "";

static const uint16_t		defaultLimitExcept = 0x1f;
static const std::string	defaultRoot = "html";
static const std::string	defaultAlias = "";

static const int32_t		defaultClientMaxBodySize = INT32_MAX;

static const std::string	defaultErrorPagePath = "";

static const int32_t		defaultListenIp = 0;
static const int32_t		defaultListenPort = 8000;
static const int32_t		defaultUriBufferSize = 8192;

static const std::string	defaultReturn= "";

#endif
