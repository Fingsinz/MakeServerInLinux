#pragma once

#include "Macros.h"
#include <arpa/inet.h>
#include <string>

class InetAddress
{
private:
	struct sockaddr_in addr;	//表示IPv4套接字地址的结构
	socklen_t addrLen;			//socket地址结构的长度

public:
	InetAddress();
	explicit InetAddress(std::string const &ip, uint16_t port);
	~InetAddress() = default;

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(InetAddress);

	/**
	 * @brief 设置地址信息
	 * @param _addr socket地址
	 * @param _addrLen socket地址长度
	 */
	void setInetAddr(sockaddr_in _addr, socklen_t _addrLen);

	/**
	 * @brief 获取socket地址
	 * @return The socket address
	 */
	sockaddr_in getAddr();

	/**
	 * @brief 获取互联网地址的长度
	 * @return socket地址的长度
	 */
	socklen_t getAddrLen();
};
