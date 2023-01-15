#include <dirent.h>
#include <unistd.h>

#include "exception/HttpErrorHandler.hpp"
#include "AutoIndex.hpp"

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
AutoIndex::autoIndex(const std::string& path) {
	DIR*			directory;
	std::string		page_content;
	struct dirent*	file_info;

	directory = opendir(path.c_str());
	if (directory == NULL)
		throw HttpErrorHandler(404);
	// it can be divided into
	// ENOENT directory not found -> 404
	// EACCESS permission denied -> 404
	// EMFILE, ENFILE, ENOMEM not enough resource -> 503 or 500
	// ENOTDIR not a directory -> logic error

	// save to string and return it OR write to socket
	page_content.reserve(2048);
	while ((file_info = readdir(directory)) != NULL) {
		page_content += "<p>";
		page_content += file_info->d_name;
		if (file_info->d_type == DT_DIR)
			page_content += "/";
		page_content += "</p>\n";
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
"</html>";
	closedir(directory);

	return page_content;
}
