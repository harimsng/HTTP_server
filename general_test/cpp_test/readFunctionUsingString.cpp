#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;


long FdGetFileSize(int fd)
{
    struct stat stat_buf;
    int rc = fstat(fd, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int ssTest(string& buffer, string path, int preBufferSize)
{
	string token;
	int fd;
	int retSize = 0;
	int readSize = 0;

	fd = open(path.c_str(), O_RDONLY);
	readSize = FdGetFileSize(fd);
	buffer.resize(preBufferSize + readSize + 1, 0);

	read(fd, &buffer[preBufferSize], readSize);

	stringstream ss(buffer);
	while (getline(ss, token, '\n'))
	{
		if (token[token.size() - 1] != '\r')
			return (retSize);
		retSize += token.size() + 1;
		cout << token.size() << endl;
		if (token == "\r")
			cout << "\\r" << endl;
		else
			cout << token << endl;
	}
	return (retSize);
}

int main(void)
{
	string	stringBuffer;
	int 	preBufferSize = 0;

	stringBuffer.erase(0, ssTest(stringBuffer, "test1.txt", preBufferSize));

	preBufferSize = stringBuffer.size() - 1;
	// cout << "string buffer : " << stringBuffer << endl;
	// cout << "cur buffer size : " << preBufferSize << endl;

	stringBuffer.erase(0, ssTest(stringBuffer, "test.txt", preBufferSize));
}
