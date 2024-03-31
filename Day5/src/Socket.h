#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include "InetAddress.h"

class Socket
{
private:
	// socket 文件描述符
	int fd;

public:
	Socket();
	explicit Socket(int);
	~Socket();

	/*
	* @brief 绑定要监听的地址
	*
	* @param addr 绑定的地址
	*/
	void bind(InetAddress const *addr);

	/*
	* @brief 监听
	*/
	void listen();

	/*
	* @brief 设置为非阻塞
	*/
	void setNonBlocking();

	/*
	* @brief 接受客户端连接
	*
	* @param addr 客户端地址
	* @return 客户端 socket fd
	*/
	int accept(InetAddress *addr);

	/*
	* @brief 获取 socket fd
	*
	* @return socket fd
	*/
	int getFd();
};
