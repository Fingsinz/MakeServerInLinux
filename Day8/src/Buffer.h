#pragma once

#include <string>

class Buffer
{
private:
	std::string buf;

public:
	Buffer() = default;
	~Buffer() = default;

	void append(char const *str, int _size);
	ssize_t size();
	char const *c_str();
	void clear();
	void getline();
};
