#include "Buffer.h"
#include <cstring>
#include <iostream>

void Buffer::append(char const *_str, int _size)
{
	for (int i = 0; i < _size; ++ i)
	{
		if (_str[i] == '\0')
			break;
		buf.push_back(_str[i]);
	}
}

ssize_t Buffer::size()
{
	return buf.size();
}

char const *Buffer::c_str()
{
	return buf.c_str();
}

void Buffer::clear()
{
	buf.clear();
}

void Buffer::getline()
{
	buf.clear();
	std::getline(std::cin, buf);
}