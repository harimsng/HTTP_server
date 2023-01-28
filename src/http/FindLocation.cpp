#include <sys/types.h>
#include <unistd.h>
#include <cstddef>

#include "Logger.hpp"
#include "FindLocation.hpp"

using namespace	std;

string
FindLocation::removeTrailingSlash(string first, string second)
{
    string newStr = first;
    if (*(first.end() - 1) == '/' && *(second.begin()) == '/')
        newStr = first.substr(0, first.find_last_of('/'));
    return newStr;
}

void
FindLocation::setRootAlias(string const &uri, VirtualServer* server)
{
    this->m_root = m_locationBlock->m_root;
    this->m_alias = m_locationBlock->m_alias;
    if (this->m_root.length() != 0) {
        this->m_root = removeTrailingSlash(this->m_root, uri);
        this->m_path = this->m_root + uri;
    }
    else if (this->m_alias.length() != 0) {
        this->m_path = this->m_alias + m_remainUri;
    }
    else {
        this->m_root = removeTrailingSlash(server->m_root, uri);
        this->m_path = server->m_root + uri;
    }
}

bool
FindLocation::findLocationBlock(Request &request, string const &uri, map<string, Location>& locationTable)
{
    string tmpUri = uri;
    int count = 0;

    while (tmpUri != "/" && tmpUri != "") {
        if ((locationTable.find(tmpUri) != locationTable.end()) && (count == 0)) {
            m_locationBlock = &locationTable[tmpUri];
            if (locationTable.find(tmpUri + "/") != locationTable.end()) {
                m_locationBlock = &locationTable[tmpUri + "/"];
            }
            m_remainUri = uri.substr(tmpUri.length());
            request.m_locationBlock = m_locationBlock;
            return true;
        }
        if (locationTable.find(tmpUri + "/") != locationTable.end() ) {
            m_locationBlock = &locationTable[tmpUri + "/"];
            m_remainUri = uri.substr(tmpUri.length());
            request.m_locationBlock = m_locationBlock;
            return true;
        }
        tmpUri = tmpUri.substr(0, tmpUri.rfind("/"));
        count++;
    }
    return false;
}

string
FindLocation::saveRealPath(Request &request, map<string, Location>& locationTable, VirtualServer* server)
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
    string uri = request.m_uri;
    LOG(DEBUG, "uri is %s", uri.data());
    if (uri == "/") // 0. root 요청
    {
        this->m_path = server->m_root;
        if (server->m_index != "")
        {
            this->m_file = server->m_index;
        }
        else
        {
            this->m_file = "index.html";
        }
        if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                m_path = m_path + "/";
        LOG(DEBUG, "0. default root %s", (m_path + m_file).data());
        request.m_path = m_path;
        request.m_file = m_file;
        return m_path + m_file;
    }
    if (uri.at(uri.size() - 1) != '/') // 1. trailing slash 없이 요청
    {
        if (findLocationBlock(request, uri, locationTable) == true) // 1-1. 있을 경우 location block의 내용으로 치환 > end
        {
            setRootAlias(uri, server);
            this->m_file = this->m_path.substr(this->m_path.rfind("/") + 1);
            LOG(DEBUG, "1. %s", m_path.data());
            if (lstat(m_path.c_str(), &d_stat) == -1) { // abcd가 없을경우
                if (request.m_method == RequestHandler::PUT)
                {
                    request.m_path = m_path.substr(0, uri.find_last_of("/")) + "/";
                    request.m_file = m_file;
                }
                else
                {
					request.m_status = 404;
                }
                LOG(DEBUG, "1-1-0. no file, no path %s", (request.m_path + request.m_file).data());
            }
            if (S_ISDIR(d_stat.st_mode) == false) { // 1-1-1 파일일 경우 > end
                request.m_path = m_root + uri.substr(0, uri.find_last_of("/")) + "/";
                request.m_file = uri.substr(uri.rfind("/") + 1);
                LOG(DEBUG, "1-1-1. %s", (request.m_path + request.m_file).data());
            }
            else  { // 1-1-2 디렉토리일 경우
                if (m_locationBlock->m_index != "")
                {
                    this->m_file = m_locationBlock->m_index;
                }
                else if (server->m_index != "")
                {
                    this->m_file = server->m_index;
                }
                else
                {
                    this->m_file = "index.html";
                }
                if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                        m_path = m_path + "/";
                string realPath = m_path + m_file;
                if (lstat(realPath.c_str(), &d_stat) == -1) {
                    this->m_file = "";
                    request.m_path = m_path;
                    request.m_file = m_file;
                    LOG(DEBUG, "1-1-2. no file, only path %s", (request.m_path + request.m_file).data());
                    return request.m_path + request.m_file;
                }
                request.m_path = m_path;
                request.m_file = m_file;
                LOG(DEBUG, "1-1-2. %s", (request.m_path + request.m_file).data());
                return request.m_path + request.m_file;
            }
        }
        else // 1-2. 없을 경우 abcd라는 파일 or 디렉토리를 찾는다
        {
            this->m_root = server->m_root;
            this->m_root = removeTrailingSlash(this->m_root, uri);
            string realPath = m_root + uri;
            if (lstat(realPath.c_str(), &d_stat) == -1) { // abcd가 없을경우
                if (request.m_method == RequestHandler::PUT)
                {
                    this->m_path = realPath.substr(0, uri.find_last_of("/")) + "/";
                    this->m_file = realPath.substr(realPath.rfind("/") + 1);
                }
                else
                {
                    this->m_path = "";
                    this->m_file = "";
                }
                request.m_path = m_path;
                request.m_file = m_file;
                LOG(DEBUG, "1-2-0. no file, no path %s", (m_path + m_file).data());
                return request.m_path + request.m_file;
            }
            if (S_ISDIR(d_stat.st_mode) == false) { // 1-2-1  abcd 파일이 있을경우 = path에 root + uri 파일부분 전까지, file에 uri 파일부분 > end
                this->m_path = m_root + uri.substr(0, uri.find_last_of("/")) + "/";
                this->m_file = uri.substr(uri.rfind("/") + 1);
                request.m_path = m_path;
                request.m_file = m_file;
                LOG(DEBUG, "1-2-1. %s", (m_path + m_file).data());
                return m_path + m_file;
            }
            else { // 1-2-2. abcd가 디렉토리일 경우 = 2로 이동
                uri = uri + "/";
            }
        }
    }
    if (findLocationBlock(request, uri, locationTable) == true) // 2-1
    {
        setRootAlias(uri, server);
        if (m_locationBlock->m_index != "")
        {
            this->m_file = m_locationBlock->m_index;
        }
        else if (server->m_index != "")
        {
            this->m_file = server->m_index;
        }
        else
        {
            this->m_file = "index.html";
        }
        if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                m_path = m_path + "/";
        string realPath = m_path + m_file;
        if (lstat(realPath.c_str(), &d_stat) == -1) {
            this->m_file = "";
            request.m_path = m_path;
            request.m_file = m_file;
            LOG(DEBUG, "2-1-0. no file, only path %s", (m_path + m_file).data());
            return m_path + m_file;
        }
        request.m_path = m_path;
        request.m_file = m_file;
        LOG(DEBUG, "2-1. %s", (m_path + m_file).data());
        return m_path + m_file;
    }
    else // 2-2
    {
        this->m_root = server->m_root;
        this->m_root = removeTrailingSlash(this->m_root, uri);
        this->m_path = this->m_root + uri;
        if (server->m_index != "")
        {
            this->m_file = server->m_index;
        }
        else
        {
            this->m_file = "index.html";
        }
        string realPath = m_path + m_file;
        if (lstat(realPath.c_str(), &d_stat) == -1) {
            this->m_file = "";
            request.m_path = m_path;
            request.m_file = m_file;
            LOG(DEBUG, "2-2-0. no file, only path %s", (m_path + m_file).data());
            return m_path + m_file;
        }
        request.m_path = m_path;
        request.m_file = m_file;
        LOG(DEBUG, "2-2. %s", (m_path + m_file).data());
        return m_path + m_file;
    }
}
