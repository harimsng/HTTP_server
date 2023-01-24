#include "FindLocation.hpp"
#include <cstddef>

using namespace	std;

void
FindLocation::setRootAlias(string const &uri, VirtualServer* server)
{
    this->m_root = m_locationBlock.m_root;
    this->m_alias = m_locationBlock.m_alias;
    if (this->m_root.length() != 0) {
        this->m_path = this->m_root + uri;
    }
    else if (this->m_alias.length() != 0) {
        this->m_path = this->m_alias + m_remainUri;
    }
    else {
        this->m_path = server->m_root + uri;
    }
}

bool
FindLocation::findLocationBlock(string const &uri, map<string, Location>& locationTable)
{
    string tmpUri = uri;
    while (tmpUri != "/") {
        if (locationTable.find(tmpUri) != locationTable.end() || 
            locationTable.find(tmpUri + "/") != locationTable.end() ) {
            m_locationBlock = locationTable[tmpUri];
            if (locationTable.find(tmpUri + "/") != locationTable.end()) {
                m_locationBlock = locationTable[tmpUri + "/"];
            }
            m_remainUri = uri.substr(tmpUri.length());
            return true;
        }
        tmpUri = tmpUri.substr(0, tmpUri.rfind("/"));
    }
    return false;
}

string
FindLocation::saveRealPath(string const &uri, map<string, Location>& locationTable, VirtualServer* server)
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
    string newUri = uri;
    if (uri.at(uri.size() - 1) != '/') // 1
    {
        if (findLocationBlock(uri, locationTable) == true) // 1-1
        {
            setRootAlias(uri, server);
            this->m_file = this->m_path.substr(this->m_path.rfind("/") + 1);
            return m_path;
        }
        else // 1-2
        {
            this->m_root = server->m_root;
            string realPath = m_root + uri;
            if (lstat(realPath.c_str(), d_stat) == -1) {
                this->m_path = "";
                this->m_file = "";
                return "";
            }
            if (S_ISDIR(d_stat->st_mode) == false) { // 1-2-1
                this->m_path = m_root + uri.substr(0, uri.find_last_of("/")) + "/";
                this->m_file = uri.substr(uri.rfind("/") + 1);
                cout << m_path + m_file << endl;
                return m_path + m_file;
            }
            else { // 1-2-2
                newUri = uri + "/";
            }
        }
    }
    if (findLocationBlock(newUri, locationTable) == true) // 2-1
    {
        setRootAlias(newUri, server);
        if (m_locationBlock.m_index != "")
        {
            this->m_file = m_locationBlock.m_index;
        }
        else if (server->m_index != "")
        {
            this->m_file = server->m_index;
        }
        else
        {
            this->m_file = "index.html";
        }
        string realPath = m_path + m_file;
        if (lstat(realPath.c_str(), d_stat) == -1) {
            this->m_file = "";
        }
        return m_path + m_file;
    }
    else // 2-2
    {
        this->m_root = server->m_root;
        this->m_path = this->m_root + newUri;
        if (server->m_index != "")
        {
            this->m_file = server->m_index;
        }
        else
        {
            this->m_file = "index.html";
        }
        string realPath = m_path + m_file;
        if (lstat(realPath.c_str(), d_stat) == -1) {
            this->m_file = "";
            return m_path + m_file;
        }
        return m_path + m_file;
    }
}
