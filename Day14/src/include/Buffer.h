#pragma once

#include "Common.h"
#include <string>

class Buffer
{
private:
	std::string mBuf;

public:
	Buffer() = default;
	~Buffer() = default;

	// 禁用拷贝和移动
	DISALLOW_COPY_AND_MOVE(Buffer);

	/**
	 * @brief 向当前字符串追加一个字符串
	 * @param str 要追加的字符串
	 * @param size 字符串大小
	 */
	void append(char const *str, int size);

	/**
	 * @brief 返回当前缓冲区字符串大小
	 * @return 字符串的大小
	 */
	ssize_t size();

	/**
	 * @brief 返回指向底层字符串数据的指针
	 * @return 指向底层字符串数据的指针
	 */
	char const *c_str();

	/**
	 * @brief 清空当前缓冲区字符串
	 */
	void clear();

	/**
	 * @brief 从控制台获取输入
	 */
	void getline();

	/**
	 * @brief 设置类的缓冲区
	 *
	 * @param buf 要设置的字符串
	 */
	void setBuf(char const *buf);
};
