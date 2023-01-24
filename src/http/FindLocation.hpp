#ifndef FINDLOCATION_HPP
#define FINDLOCATION_HPP

#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Location.hpp"
#include "VirtualServer.hpp"

class FindLocation {
    public:
        std::string m_root;
        std::string m_alias;
        std::string m_path;
        std::string m_file;
        std::string m_remainUri;
        Location    m_locationBlock;
        struct stat*   d_stat;

        std::string removeTrailingSlash(string first, string second);
        void setRootAlias(std::string const &uri, VirtualServer* server);
        bool findLocationBlock(Request &request, std::string const &uri, std::map<std::string, Location>& locationTable);
        std::string saveRealPath(Request &request, std::map<std::string, Location>& locationTable, VirtualServer* server);
};

#endif
