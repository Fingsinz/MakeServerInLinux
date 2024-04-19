#pragma once

#include <string>

class Buffer
{
private:
	std::string buf;

public:
	Buffer() = default;
	~Buffer() = default;

	/**
	 * @brief 向当前字符串追加一个字符串
	 * @param str 要追加的字符串
	 * @param _size 字符串大小
	 */
	void append(char const *str, int _size);

	/**
	 * @brief 返回当前缓冲区字符串大小
	 * @return 字符串的大小
	 */
	ssize_t size();

	/**
	 * @brief返回指向底层字符串数据的指针
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

	void setBuf(char const *_buf);
};
