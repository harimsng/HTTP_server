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
FindLocation::setRootAlias(string const &uri)
{
    m_root = m_locationBlock->m_root;
    m_alias = m_locationBlock->m_alias;

	if (m_alias.length() != 0)
	{
		m_path = m_alias + m_remainUri;
	}
	else
	{
        m_root = removeTrailingSlash(m_root, uri);
        m_path = m_root + uri;
    }
}

bool
FindLocation::findLocationBlock(Request &request, string const &uri, map<string, Location>& locationTable)
{
    string tmpUri = uri;
    string originUri = uri;
    int count = 0;

	if (((locationTable.find(tmpUri) != locationTable.end()) || (locationTable.find(tmpUri + "/") != locationTable.end())) && (count == 0)) {
		m_locationBlock = &locationTable[tmpUri];
		if (locationTable.find(tmpUri + "/") != locationTable.end()) {
			m_locationBlock = &locationTable[tmpUri + "/"];
		}
		m_remainUri = uri.substr(tmpUri.length());
		request.m_locationBlock = m_locationBlock;
		return true;
	}
    while (tmpUri != "") {
        if (locationTable.find(tmpUri) != locationTable.end() ) {
            m_locationBlock = &locationTable[tmpUri];
            m_remainUri = originUri.substr(tmpUri.length());
            request.m_locationBlock = m_locationBlock;
            return true;
        }
		if (tmpUri == "/")
			return false;
        tmpUri = tmpUri.substr(0, tmpUri.rfind("/", tmpUri.size() - 2) + 1);
        count++;
    }
    return false;
}

string
FindLocation::saveRealPath(Request &request, map<string, Location>& locationTable, VirtualServer* server)
{
    string uri = request.m_uri;
    if (uri == "/") // 0. root 요청
    {
        if ((locationTable.find("/") != locationTable.end()))
        {
            m_locationBlock = &locationTable["/"];
            request.m_locationBlock = m_locationBlock;
            m_root = m_locationBlock->m_root;
            m_alias = m_locationBlock->m_alias;
            if (m_root.length() != 0) {
                m_root = removeTrailingSlash(m_root, uri);
                m_path = m_root + uri;
            }
            else if (m_alias.length() != 0) {
                m_path = m_alias + m_remainUri;
            }
            else {
                m_root = removeTrailingSlash(server->m_root, uri);
                m_path = server->m_root + uri;
            }
            if (m_locationBlock->m_index.size() != 0 &&
					(request.m_method != RequestHandler::PUT && request.m_method != RequestHandler::POST))
            {
                for (size_t i = 0; i < m_locationBlock->m_index.size(); i++)
                {
                    m_file = m_locationBlock->m_index[i];
                    if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                        m_path = m_path + "/";
                    if (lstat(m_path.c_str(), &d_stat) == -1) {
                        request.m_path = m_path;
                        request.m_file = m_file;
                        request.m_status = 404;
                        LOG(DEBUG, "0-1-1. no file, no path %s", (request.m_path + request.m_file).data());
                        return request.m_path + request.m_file;
                    }
                    string realPath = m_path + m_file;
                    if (lstat(realPath.c_str(), &d_stat) != -1) {
                        request.m_path = m_path;
                        request.m_file = m_file;
                        LOG(DEBUG, "0-1-2. %s", (request.m_path + request.m_file).data());
                        return request.m_path + request.m_file;
                    }
                }
            }
            m_file = "";
            request.m_path = m_path;
            request.m_file = m_file;
            request.m_status = 404;
            LOG(DEBUG, "0-1-2. no file, only path %s", (request.m_path + request.m_file).data());
            return request.m_path + request.m_file;
        }
        else
        {
            m_path = server->m_root;
            if (server->m_index.size() != 0 &&
					(request.m_method != RequestHandler::PUT && request.m_method != RequestHandler::POST))
            {
                for (size_t i = 0; i < server->m_index.size(); i++)
                {
                    m_file = server->m_index[i];
                    if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                        m_path = m_path + "/";
                    if (lstat(m_path.c_str(), &d_stat) == -1) {
                        request.m_path = m_path;
                        request.m_file = m_file;
                        request.m_status = 404;
						LOG(DEBUG, "file=\"%s\", path=\"%s\"", m_path.c_str(), m_file.c_str());
                        LOG(DEBUG, "0-2-1. no file, no path %s", (m_path + m_file).data());
                        return request.m_path + request.m_file;
                    }
                    string realPath = m_path + m_file;
                    if (lstat(realPath.c_str(), &d_stat) != -1) {
                        request.m_path = m_path;
                        request.m_file = m_file;
                        LOG(DEBUG, "0-2-2. %s", (m_path + m_file).data());
                        return request.m_path + request.m_file;
                    }
                }
            }
            m_file = "";
            request.m_path = m_path;
            request.m_file = m_file;
            request.m_status = 404;
            LOG(DEBUG, "0-2-1. no file, only path %s", (m_path + m_file).data());
            return m_path + m_file;
        }
    }
    if (uri.at(uri.size() - 1) != '/') // 1. trailing slash 없이 요청
    {
        if (findLocationBlock(request, uri, locationTable) == true) // 1-1. 있을 경우 location block의 내용으로 치환
        {
            setRootAlias(uri);
            m_file = m_path.substr(m_path.rfind("/") + 1);
            LOG(DEBUG, "1. %s", m_path.data());
            if (lstat(m_path.c_str(), &d_stat) == -1) { // abcd가 없을경우
                request.m_path = m_path.substr(0, m_path.find_last_of("/")) + "/";
                request.m_file = m_file;
				if (request.m_method != RequestHandler::PUT)
				{
					request.m_status = 404;
				}
                LOG(DEBUG, "1-1-0. no file, no path %s", (request.m_path + request.m_file).data());
                return request.m_path + request.m_file;
            }
            if (S_ISDIR(d_stat.st_mode) == false) { // 1-1-1 파일일 경우 > end
                request.m_path = m_path.substr(0, m_path.find_last_of("/")) + "/";
				request.m_file = m_file;
                LOG(DEBUG, "1-1-1. %s", (request.m_path + request.m_file).data());
                return request.m_path + request.m_file;
            }
            else  { // 1-1-2 디렉토리일 경우
                if (m_locationBlock->m_index.size() != 0 &&
						(request.m_method != RequestHandler::PUT && request.m_method != RequestHandler::POST))
                {
                    for (size_t i = 0; i < m_locationBlock->m_index.size(); i++)
                    {
                        m_file = m_locationBlock->m_index[i];
                        if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                            m_path = m_path + "/";
                        if (lstat(m_path.c_str(), &d_stat) == -1) {
                            request.m_path = m_path;
                            request.m_file = m_file;
                            request.m_status = 404;
                            LOG(DEBUG, "1-1-2. no file, no path %s", (request.m_path + request.m_file).data());
                            return request.m_path + request.m_file;
                        }
                        string realPath = m_path + m_file;
                        if (lstat(realPath.c_str(), &d_stat) != -1) {
                            request.m_path = m_path;
                            request.m_file = m_file;
                            LOG(DEBUG, "1-1-2. %s", (request.m_path + request.m_file).data());
                            return request.m_path + request.m_file;
                        }
                    }
                }
                m_file = "";
                request.m_path = m_path;
                request.m_file = m_file;
                request.m_status = 404;
                LOG(DEBUG, "1-1-2. no file, only path %s", (request.m_path + request.m_file).data());
                return request.m_path + request.m_file;
            }
        }
        else // 1-2. 없을 경우 abcd라는 파일 or 디렉토리를 찾는다
        {
            m_root = server->m_root;
            m_root = removeTrailingSlash(m_root, uri);
            string realPath = m_root + uri;
            if (lstat(realPath.c_str(), &d_stat) == -1) { // abcd가 없을경우
                m_path = realPath.substr(0, uri.find_last_of("/")) + "/";
                m_file = realPath.substr(realPath.rfind("/") + 1);
				if (request.m_method != RequestHandler::PUT)
				{
					request.m_status = 404;
				}
                request.m_path = m_path;
                request.m_file = m_file;
                LOG(DEBUG, "1-2-0. no file, no path %s", (m_path + m_file).data());
                return request.m_path + request.m_file;
            }
            if (S_ISDIR(d_stat.st_mode) == false) { // 1-2-1  abcd 파일이 있을경우 = path에 root + uri 파일부분 전까지, file에 uri 파일부분 > end
                m_path = m_root + uri.substr(0, uri.find_last_of("/")) + "/";
                m_file = uri.substr(uri.rfind("/") + 1);
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
        setRootAlias(uri);

        if (m_locationBlock->m_index.size() != 0 &&
				(request.m_method != RequestHandler::PUT && request.m_method != RequestHandler::POST))
        {
            for (size_t i = 0; i < m_locationBlock->m_index.size(); i++)
            {
                m_file = m_locationBlock->m_index[i];
                if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                    m_path = m_path + "/";
                if (lstat(m_path.c_str(), &d_stat) == -1) {
                    request.m_path = m_path;
                    request.m_file = m_file;
                    request.m_status = 404;
                    LOG(DEBUG, "2-1-0. no file, no path %s", (m_path + m_file).data());
                    return request.m_path + request.m_file;
                }
                string realPath = m_path + m_file;
                if (lstat(realPath.c_str(), &d_stat) != -1) {
                    request.m_path = m_path;
                    request.m_file = m_file;
                    LOG(DEBUG, "2-1. %s", (m_path + m_file).data());
                    return request.m_path + request.m_file;
                }
            }
        }
        m_file = "";
        request.m_path = m_path;
        request.m_file = m_file;
        request.m_status = 404;
        LOG(DEBUG, "2-1-0. no file, only path %s", (m_path + m_file).data());
        return m_path + m_file;
    }
    else // 2-2
    {
        m_root = server->m_root;
        m_root = removeTrailingSlash(m_root, uri);
        m_path = m_root + uri;

        if (server->m_index.size() != 0 &&
				(request.m_method != RequestHandler::PUT && request.m_method != RequestHandler::POST))
        {
            for (size_t i = 0; i < server->m_index.size(); i++)
            {
                m_file = server->m_index[i];
                if (*(m_path.end() - 1) != '/' && *(m_file.begin()) != '/')
                    m_path = m_path + "/";
                if (lstat(m_path.c_str(), &d_stat) == -1) {
                    request.m_path = m_path;
                    request.m_file = m_file;
                    request.m_status = 404;
                    LOG(DEBUG, "2-2-0. no file, no path %s", (m_path + m_file).data());
                    return request.m_path + request.m_file;
                }
                string realPath = m_path + m_file;
                if (lstat(realPath.c_str(), &d_stat) != -1) {
                    request.m_path = m_path;
                    request.m_file = m_file;
                    LOG(DEBUG, "2-2. %s", (m_path + m_file).data());
                    return request.m_path + request.m_file;
                }
            }
        }
        m_file = "";
        request.m_path = m_path;
        request.m_file = m_file;
        request.m_status = 404;
        LOG(DEBUG, "2-2-0. no file, only path %s", (m_path + m_file).data());
        return m_path + m_file;
    }
}
