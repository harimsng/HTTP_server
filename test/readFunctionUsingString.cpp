#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <unistd.h>

using namespace std;

void ssTest(string& buffer, string path, size_t readSize)
{
	string token;
	int fd;

	fd = open(path.c_str(), O_RDONLY);
	read(fd, &buffer[0], readSize);
	stringstream ss(buffer);
	while (getline(ss, token, '1'))
	{
		cout << ss.fail() << endl;
		cout << token << endl;
	}

}

int main(void)
{
	string stringBuffer;

	stringBuffer.resize(22, 0);

	ssTest(stringBuffer, "test1.txt", 22);
}
