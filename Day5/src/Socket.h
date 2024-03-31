#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include "InetAddress.h"

class Socket
{
private:
	// socket �ļ�������
	int fd;

public:
	Socket();
	explicit Socket(int);
	~Socket();

	/*
	* @brief ��Ҫ�����ĵ�ַ
	*
	* @param addr �󶨵ĵ�ַ
	*/
	void bind(InetAddress const *addr);

	/*
	* @brief ����
	*/
	void listen();

	/*
	* @brief ����Ϊ������
	*/
	void setNonBlocking();

	/*
	* @brief ���ܿͻ�������
	*
	* @param addr �ͻ��˵�ַ
	* @return �ͻ��� socket fd
	*/
	int accept(InetAddress *addr);

	/*
	* @brief ��ȡ socket fd
	*
	* @return socket fd
	*/
	int getFd();
};
