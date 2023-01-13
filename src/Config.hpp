#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

struct	Config
{
	static const std::string	s_defaultIndex;
	static const bool			s_defaultAutoindex;
	static const std::string	s_defaultExpires;
	static const std::string	s_defaultProxyPass;
	static const std::string	s_defaultLimitExcept;
	static const std::string	s_defaultPath;
	static const std::string	s_defaultRoot;
	static const std::string	s_defaultCgiPass;
	static const std::string	s_defaultAlias;
	static const std::string	s_defaultClientMaxBodySize;
//	static const std::vector<std::string>	m_serverNames;
//	default servername is set in config parser
	static const std::string	m_errorCode;
	static const std::string	m_errorPath;
	static const std::string	m_errorPages;
	static const sockaddr_in	m_listen;
	static const int32_t		m_uriBufferSize;

};

#endif
