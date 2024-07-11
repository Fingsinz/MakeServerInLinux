#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
	//表示IPv4套接字地址的结构
	struct sockaddr_in addr;

	//socket地址结构的长度
	socklen_t addrLen;

	InetAddress();
	explicit InetAddress(std::string const &ip, uint16_t port);
	~InetAddress();
};
