#pragma once

#include "Macros.h"
#include "sys/socket.h"

class InetAddress;

class Socket
{
private:
	int fd{ -1 };	// socket 文件描述符

public:
	Socket();
	explicit Socket(int);
	~Socket();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Socket);

	/*
	 * @brief 绑定要监听的地址
	 *
	 * @param _addr 绑定的地址
	 */
	void bind(InetAddress *_addr);

	/*
	 * @brief 监听
	 */
	void listen();

	/*
	 * @brief 设置为非阻塞
	 */
	void setNonBlocking();

	/*
	 * @brief 判断是否为非阻塞
	 * 
	 * @return 是否为非阻塞
	 */
	bool isNonBlocking();

	/*
	 * @brief 接受客户端连接
	 *
	 * @param _addr 客户端地址
	 * @return 客户端 socket fd
	 */
	int accept(InetAddress *_addr);

	/**
	 * @brief 连接到指定的地址
	 *
	 * @param _addr 要连接的InetAddress。
	 */
	void connect(InetAddress *_addr);

	/*
	 * @brief 获取 socket fd
	 *
	 * @return socket fd
	 */
	int getFd();
};
