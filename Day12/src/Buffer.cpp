#include "Buffer.h"
#include <cstring>
#include <iostream>

void Buffer::append(char const *str, int size)
{
	for (int i = 0; i < size; ++ i)
	{
		if (str[i] == '\0')
			break;
		mBuf.push_back(str[i]);
	}
}

ssize_t Buffer::size()
{
	return mBuf.size();
}

char const *Buffer::c_str()
{
	return mBuf.c_str();
}

void Buffer::clear()
{
	mBuf.clear();
}

void Buffer::getline()
{
	mBuf.clear();
	std::getline(std::cin, mBuf);
}

void Buffer::setBuf(char const *buf)
{
	mBuf.clear();
	mBuf.append(buf);
}