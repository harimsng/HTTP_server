#include <dirent.h>
#include <unistd.h>
#include <string>

#include "exception/HttpErrorHandler.hpp"
#include "AutoIndex.hpp"

using namespace std;

// deleted
AutoIndex&
AutoIndex::operator=(const AutoIndex& autoIndex)
{
	(void)autoIndex;
	return *this;
}

AutoIndex::AutoIndex(const AutoIndex& autoIndex)
{
	(void)autoIndex;
}

// constructors & destructor
AutoIndex::AutoIndex()
{
}

AutoIndex::~AutoIndex()
{
}

std::string
AutoIndex::autoIndex(const std::string& path, const std::string& uri) {
	DIR*			directory;
	std::string		page_content;
	std::string		link;
	struct dirent*	file_info;

	directory = opendir(path.c_str());
	// if (directory == NULL)
	//	throw HttpErrorHandler(404);

	// it can be divided into
	// ENOENT directory not found -> 404
	// EACCESS permission denied -> 404
	// EMFILE, ENFILE, ENOMEM not enough resource -> 503 or 500
	// ENOTDIR not a directory -> logic error

	// save to string and return it OR write to socket

	// Users/soum/webserv/html/44/
	page_content.reserve(2048);
	while ((file_info = readdir(directory)) != NULL) {
		link = file_info->d_name;
		if (file_info->d_type == DT_DIR)
			link += "/";
		page_content += "<div style=\"margin-top:10px; font-size:20px;\">\n";
		page_content += "<a href=\"" + uri + link + "\">";
		page_content += link;
		page_content += "</a>\n</div>\n";
	}
	page_content =
"<!DOCTYPE html>\n"
"<html>\n"
"	<head>\n"
"		<title>Index</title>\n"
"	</head>\n"
"	<body>\n"
"		<h1>"
+ path +
"		</h1>\n"
+ page_content +
"	</body>\n"
"</html>\n";
	closedir(directory);

	return page_content;
}
