#include "InetAddress.h"
#include <cstring>

InetAddress::InetAddress() : addrLen(sizeof(addr))
{
	bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(std::string const &ip, uint16_t port) : addrLen(sizeof(addr))
{
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
}

InetAddress::~InetAddress()
{}