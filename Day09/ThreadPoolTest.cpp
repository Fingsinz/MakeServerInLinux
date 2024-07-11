#include <iostream>
#include <string>
#include "src/ThreadPool.h"

void print(int a, double b, char const *c, std::string d)
{
	std::cout << a << b << c << d << "\n";
}

void test()
{
	std::cout << "Test\n";
}

int main()
{
	ThreadPool *pool = new ThreadPool();
	std::function<void()> func = std::bind(print, 1, 2.0, "Hello", "World");
	pool->add(func);
	func = test;
	pool->add(func);
	delete pool;
	return 0;
}