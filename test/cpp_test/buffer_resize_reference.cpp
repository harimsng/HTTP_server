#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

class A{
	public:
	A(){
		buffer.resize(100);
		buffer = "42";
	};

	void resize(void)
	{
		buffer.resize(1000);
		buffer = "24";
	}

	string& getBuffer(void)
	{
		return (buffer);
	}

	void	printBuffer(void)
	{
		cout << "buffer : " << buffer << endl;
		cout << "ptr : " << (void*)&buffer[0] << endl;
	}
	private:
	string buffer;
};

class B
{
	public:
		B(string& buffer) : buffer(&buffer)
		{};
		void resize(void)
		{
			buffer->resize(400);
			*buffer = "2424";
		}
		void printBuffer(void)
		{
			cout << "buffer : " << *buffer << endl;
			cout << "ptr : " << (void*)buffer << endl;
		}

	private:
		string* buffer;
};

int main(void)
{
	string	a;

	a.reserve(1000);
	std::cout << a.capacity() << '\n';
//	A test1;
//
//	B test2(test1.getBuffer());
//
//	test1.printBuffer();
//	test2.printBuffer();
//
//	test2.resize();
//	test1.printBuffer();
//	test2.printBuffer();
//
//	test1.resize();
//	test1.printBuffer();
//	test2.printBuffer();
//
//
//	test2.resize();
//	test1.printBuffer();
//	test2.printBuffer();
}
