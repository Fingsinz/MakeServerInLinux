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
	 * @brief ��ǰ�ַ���׷��һ���ַ���
	 * @param str Ҫ׷�ӵ��ַ���
	 * @param _size �ַ�����С
	 */
	void append(char const *str, int _size);

	/**
	 * @brief ���ص�ǰ�������ַ�����С
	 * @return �ַ����Ĵ�С
	 */
	ssize_t size();

	/**
	 * @brief����ָ��ײ��ַ������ݵ�ָ��
	 * @return ָ��ײ��ַ������ݵ�ָ��
	 */
	char const *c_str();

	/**
	 * @brief ��յ�ǰ�������ַ���
	 */
	void clear();

	/**
	 * @brief �ӿ���̨��ȡ����
	 */
	void getline();

	void setBuf(char const *_buf);
};
