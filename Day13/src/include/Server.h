#pragma once

#include "Macros.h"
#include <functional>
#include <map>
#include <vector>

class Acceptor;
class EventLoop;
class Connection;
class Socket;
class ThreadPool;

class Server
{
private:
	EventLoop *mainReactor;									// 只负责接受连接，然后分发给一个subReactor
	Acceptor *acceptor;										// 连接接受器
	std::vector<EventLoop *> subReactors;					// 负责处理事件循环
	std::map<int, Connection *> connections;				// 存储连接及其相应的文件描述符
	ThreadPool *threadPool;									// 线程池
	std::function<void(Connection *)> onConnectCallback;	// 连接的业务

public:
	explicit Server(EventLoop *loop);
	~Server();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Server);

	/**
	 * @brief 处理给定文件描述符的读取事件
	 *
	 * @param fd 要读取的文件描述
	 */
	void onConnect(std::function<void(Connection *)> onConnectCallback);

	/**
	 * @brief 处理与所提供套接字的新连接
	 *
	 * @param _socket 表示新连接的套接字
	 */
	void newConnection(Socket *socket);

	/**
	 * @brief 断开与提供的套接字关联的连接
	 *
	 * @param _sockfd 需要删除其连接的套接字文件描述符
	 */
	void deleteConnection(int sockfd);
};
