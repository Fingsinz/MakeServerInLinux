#pragma once

#include <vector>

class Channel;
class EventLoop;
class Socket;
class Server;
class InetAddress;

class Server
{
private:
	// 指向EventLoop对象的指针
	EventLoop *loop;

	// 服务器套接字
	Socket *serverSock;

	// 服务器地址
	InetAddress *serverAddr;

	// 服务器通道
	Channel *serverChannel;

	// 保存客户端的套接字
	std::vector<std::pair<Socket *, InetAddress *>> clients;

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
