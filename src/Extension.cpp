#include "Extension.hpp"
#include <utility>

using namespace std;

std::vector<std::pair<std::string, std::string> > extensionType = {
	std::pair<std::string, std::string>("html", "text/html"),
	std::pair<std::string, std::string>("htm","text/html"),
	std::pair<std::string, std::string>("shtml","text/html"),
	std::pair<std::string, std::string>("css","text/css"),
	std::pair<std::string, std::string>("xml","text/xml"),
	std::pair<std::string, std::string>("gif","image/gif"),
	std::pair<std::string, std::string>("jpeg","image/gif"),
	std::pair<std::string, std::string>("jpg","image/jpeg"),
	std::pair<std::string, std::string>("txt","text/plain"),
	std::pair<std::string, std::string>("png","image/png"),
	std::pair<std::string, std::string>("ico","image/x-icon"),
	std::pair<std::string, std::string>("bmp","image/x-ms-bmp"),
	std::pair<std::string, std::string>("svg","image/x-ms-bmp"),
	std::pair<std::string, std::string>("webp","image/webp"),
	std::pair<std::string, std::string>("mp4", "video/mp4"),
	std::pair<std::string, std::string>("mpeg", "video/mp4"),
	std::pair<std::string, std::string>("mpg", "video/mpeg"),
	std::pair<std::string, std::string>("avi", "video/x-msvideo"),
	std::pair<std::string, std::string>("js","application/javascript"),
	std::pair<std::string, std::string>("woff","application/font-woff"),
	std::pair<std::string, std::string>("json","application/json"),
	std::pair<std::string, std::string>("doc","application/msword"),
	std::pair<std::string, std::string>("pdf","application/pdf"),
	std::pair<std::string, std::string>("xls", "application/vnd.ms-excel"),
	std::pair<std::string, std::string>("rar", "application/x-rar-compressed"),
	std::pair<std::string, std::string>("zip", "application/zip"),
	std::pair<std::string, std::string>("7z", "application/x-7z-compressed"),
	std::pair<std::string, std::string>("bin", "application/zip"),
	std::pair<std::string, std::string>("exe", "application/zip"),
	std::pair<std::string, std::string>("mp3", "audio/mpeg"),
	std::pair<std::string, std::string>("ogg", "audio/ogg"),
	std::pair<std::string, std::string>("m4a", "audio/x-m4a")
};

std::string findContentType(std::string content)
{
	std::string extension;

	extension = content.substr(content.find('.') + 1);
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it = extensionType.begin(); it != extensionType.end(); it++)
		if (extension == it->first)
			return (it->second);
	extension = "text/plain";
	return (extension);
}
