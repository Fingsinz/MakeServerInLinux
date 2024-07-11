#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
	//��ʾIPv4�׽��ֵ�ַ�Ľṹ
	struct sockaddr_in addr;

	//socket��ַ�ṹ�ĳ���
	socklen_t addrLen;

	InetAddress();
	explicit InetAddress(std::string const &ip, uint16_t port);
	~InetAddress();
};
