#include "AutoIndex.hpp"
#include <dirent.h>

// constructors & destructor
AutoIndex::AutoIndex()
{
}

AutoIndex::~AutoIndex()
{
}

AutoIndex::AutoIndex(const AutoIndex& autoIndex)
{
}

// operators
AutoIndex&
AutoIndex::operator=(const AutoIndex& autoIndex)
{
	return *this;
}

std::string
AutoIndex::autoIndex(std::string path) {
	std::string		current_path;
	DIR*			directory;
	std::string		page_content;
	struct dirent*	file_info;

	current_path = path;
	directory = opendir(current_path.c_str());
	page_content += "<!DOCTYPE html>\n";
	page_content += "<html>\n";
	page_content += "	<head>\n";
	page_content += "		<title>Index</title>\n";
	page_content += "	</head>\n";
	page_content += "	<body>\n";
	page_content += "		<h1>";
	page_content += 			path;
	page_content += "		</h1>\n";
	
	if (directory == NULL) {
		closedir(directory);
		page_content += "</body>\n";
		page_content += "</html>";
		return page_content;
	}

	while ((file_info = readdir(directory)) != NULL) {
		page_content += "<p>";
		if (file_info->d_type == DT_DIR) {
			page_content += file_info->d_name;
			page_content += "/";
		}
		else if (file_info->d_type == DT_REG) {
			page_content += file_info->d_name;
		}
		page_content += "</p>\n";
	}
	closedir(directory);
	page_content += "</body>\n";
	page_content += "</html>";

	return page_content;
}