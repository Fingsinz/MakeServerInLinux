#pragma once

#include <functional>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection
{
private:

	// EventLoop指针
	EventLoop *loop;
	// Socket指针
	Socket *sock;
	// Channel指针
	Channel *channel;
	// 删除连接的回调函数
	std::function<void(Socket *)> deleteConnectionCallback;
	// 读缓冲区
	Buffer *readBuffer;

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
	void setDeleteConnectionCallback(std::function<void(Socket *)> _callback);
};
