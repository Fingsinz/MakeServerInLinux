#pragma once

#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
private:
	// 用于事件处理的EventLoop指针
	EventLoop *loop;

	// 用于处理套接字操作的套接字指针
	Socket *sock;

	// 用于存储地址信息的指针
	InetAddress *addr;

	// 用于接受连接的 Channel 指针
	Channel *acceptChannel;

	// 定义一个新建连接的回调函数
	std::function<void(Socket *)> newConnectionCallback;

public:
	Acceptor(EventLoop *_loop);
	~Acceptor();

	/**
	 * @brief 接受新连接
	 */
	void acceptConnection();

	/**
	 * @brief 设置新连接的回调函数。
	 *
	 * @param _callback 为新连接设置的回调函数。
	 */
	void setNewConnectionCallback(std::function<void(Socket *)> _callback);
};
