#pragma once

#include <functional>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection
{
private:
	EventLoop *loop;	// EventLoop指针
	Socket *sock;		// Socket指针
	Channel *channel;	// Channel指针
	Buffer *readBuffer;	// 读缓冲区

	// 删除连接的回调函数
	std::function<void(int)> deleteConnectionCallback;

public:
	Connection(EventLoop *_loop, Socket *_sock);
	~Connection();

	/**
	 * @brief 回显sockfd发来的数据
	 *
	 * @param sockfd 通过套接字文件描述符发回数据
	 */
	void echo(int sockfd);

	/**
	 * @brief 设置删除连接时要调用的回调函数
	 *
	 * @param _callback 删除连接时要调用的回调函数
	 */
	void setDeleteConnectionCallback(std::function<void(int)> _callback);

	/**
	 * @brief 通过指定的套接字发送数据。
	 *
	 * @param sockfd 套接字文件描述符
	 */
	void send(int sockfd);
};
