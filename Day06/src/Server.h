#pragma once

#include <vector>

class Acceptor;
class EventLoop;
class Socket;

class Server
{
private:
	// 指向EventLoop对象的指针
	EventLoop *loop;

	// 指向Acceptor对象的指针
	Acceptor *acceptor;

public:
	Server(EventLoop *_loop);
	~Server();

	/**
	 * @brief 处理给定文件描述符的读取事件
	 *
	 * @param fd 要读取的文件描述
	 */
	void handleReadEvent(int fd);

	/**
	 * @brief 处理与所提供套接字的新连接
	 *
	 * @param _socket 表示新连接的套接字
	 */
	void newConnection(Socket *_socket);
};
