#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
	struct sockaddr_in addr;
	socklen_t addrLen;

	InetAddress();
	explicit InetAddress(std::string const &ip, uint16_t port);
	~InetAddress();
};
