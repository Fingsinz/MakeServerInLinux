#pragma once

class InetAddress;

class Socket
{
private:
	int fd{ -1 };	// socket 文件描述符

public:
	Socket();
	explicit Socket(int);
	~Socket();

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
