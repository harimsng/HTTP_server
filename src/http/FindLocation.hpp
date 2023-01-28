#ifndef FINDLOCATION_HPP
#define FINDLOCATION_HPP

#include <sys/stat.h>
#include <string>
#include <map>
#include <vector>

#include "VirtualServer.hpp"

class FindLocation {
    public:
        std::string m_root;
        std::string m_alias;
        std::string m_path;
        std::string m_file;
        std::string m_remainUri;
        Location    m_locationBlock;
        struct stat   d_stat;

        std::string removeTrailingSlash(std::string first, std::string second);
        void setRootAlias(std::string const &uri, VirtualServer* server);
        bool findLocationBlock(Request &request, std::string const &uri, std::map<std::string, Location>& locationTable);
        std::string saveRealPath(Request &request, std::map<std::string, Location>& locationTable, VirtualServer* server);
};

#endif
