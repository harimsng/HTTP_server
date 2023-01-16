#ifndef REQUEST_SUDO_HPP
#define REQUEST_SUDO_HPP

#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class RequestSudo {
    public:
        std::string root;
        std::string alias;
        std::string path;
        std::string file;
        std::string remainUri;
        LocationBlock locationBlock;
        struct stat*   d_stat;

        void setRootAlias(std::string const &uri);
        bool find_location_block(std::string const &uri);
        void saveRealPath(std::string const &uri);
};

#endif