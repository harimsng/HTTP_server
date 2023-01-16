#include "request_sudo.hpp"
#include <cstddef>

void
RequestSudo::setRootAlias(std::string const &uri)
{
    this->root = locationBlock->root;
    this->alias = locationBlock->alias;
    if (this->root.length() != 0) {
        this->path = this->root + uri;
    }
    else if (this->alias.length() != 0) {
        this->path = this->alias + remainUri;
    }
    else {
        this->path = serverRoot + uri;
    }
}

bool
RequestSudo::find_location_block(std::string const &uri)
{
    std::string tmpUri = uri;
    while (tmpUri != "/") {
        if (location_blocks.find(tmpUri) != location_blocks.end() || 
            location_blocks.find(tmpUri + "/") != location_blocks.end() ) {
            locationBlock = location_blocks[tmpUri];
            if (location_blocks.find(tmpUri + "/") != location_blocks.end()) {
                locationBlock = location_blocks[tmpUri + "/"];
            }
            remainUri = uri.substr(tmpUri.length());
            return true;
        }
        tmpUri = tmpUri.substr(0, tmpUri.rfind("/"));
    }
    return false;
}

void
RequestSudo::saveRealPath(std::string const &uri)
{
    /*
    trailing slash 없이 요청 (/abcd)(/abcd/efgh/ijkl)
        1. /abcd location block 찾기
        1-1. 있을 경우 location block의 내용으로 치환 > end
        1-2. 없을 경우 abcd라는 파일 or 디렉토리를 찾는다

            1-2-1. abcd 파일이 있을경우 = path에 root + uri 파일부분 전까지, file에 uri 파일부분 > end
            1-2-2. abcd가 디렉토리일 경우 = 2로 이동

        2. /abcd/ location block 찾기
        2-1. 있을 경우 location block의 내용으로 치환
            2-1-1. 블록 내에 index가 존재 = file에 index추가
            2-1-2. 없을경우 최상위의 index를 file에 추가
            2-1-3. 둘다 없을경우 index.html를 file에 추가
            2-1-4. 만약 path + file 가 존재 하지 않을경우 = path만 변수에 저장, file 비움 > end
        2-2. 없을 경우 root + /abcd/ 라는 경로를 path에 추가
            2-2-1. 최상위의 index를 file에 추가
            2-2-2. 없을경우 index.html를 file에 추가
            2-1-4. 만약 path + file 가 존재 하지 않을경우 = path만 변수에 저장, file 비움 > end
    */
    std::string newUri = "";
    if (trailing_slash == false) // 1
    {
        if (find_location_block(uri) == true) // 1-1
        {
            setRootAlias(uri);
            this->file = this->path.substr(this->path.rfind("/") + 1);
            return ;
        }
        else // 1-2
        {
            this->root = serverRoot->root;
            std::string realPath = root + uri;
            if (lstat(realPath.c_str(), d_stat) == -1) {
                this->path = "";
                this->file = "";
                return ;
            }
            if (S_ISDIR(d_stat->st_mode) == false) { // 1-2-1
                this->path = root + uri.substr(0, uri.find_last_of("/")) + "/";
                this->file = uri.substr(uri.rfind("/") + 1);
                return ;
            }
            else { // 1-2-2
                newUri = uri + "/";
            }
        }
    }
    if (find_location_block(uri) == true) // 2-1
    {
        setRootAlias(uri);
        if (locationBlock->index != "")
        {
            this->file = locationBlock->index;
        }
        else if (serverRoot->index != "")
        {
            this->file = serverRoot->index;
        }
        else
        {
            this->file = "index.html";
        }
        std::string realPath = path + file;
        if (lstat(realPath.c_str(), d_stat) == -1) {
            this->file = "";
        }
        return ;
    }
    else // 2-2
    {
        this->root = serverRoot->root;
        this->path = this->root + uri;
        if (serverRoot->index != "")
        {
            this->file = serverRoot->index;
        }
        else
        {
            this->file = "index.html";
        }
        std::string realPath = path + file;
        if (lstat(realPath.c_str(), d_stat) == -1) {
            this->file = "";
            return ;
        }
    }
}