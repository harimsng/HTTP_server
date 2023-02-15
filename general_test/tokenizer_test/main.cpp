#include <iostream>
#include <fstream>
#include <sstream>

#include "HttpStreamTokenizer.hpp"

using namespace std;

string readBuffer(string event)
{
	ifstream file;
	stringstream buffer;

	file.open(event);
	buffer << file.rdbuf();
	file.close();
	return (buffer.str());
}

int main(void)
{
	string buffer;

	HttpStreamTokenizer tokenizer;

	tokenizer.init(buffer);

	buffer += readBuffer("event1.txt");
	if (tokenizer.updateBuffer() != string::npos)
	{
		while (tokenizer.empty() == false)
			cout << "$" << tokenizer.get() << "$" << endl;
	}

	buffer += readBuffer("event2.txt");
	if (tokenizer.updateBuffer() != string::npos)
	{
		while (tokenizer.empty() == false)
			cout << "$" << tokenizer.get() << "$" << endl;
	}
}
