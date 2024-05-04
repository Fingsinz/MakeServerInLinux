#pragma once

#include "Macros.h"
#include <functional>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection
{
public:
	enum State
	{
		Invalid = 1, // 初始无效状态
		Handshaking, // 握手过程中的状态
		Connected,   // 连接建立
		Closed,      // 连接关闭
		Failed,      // 连接失败
	};

private:
	EventLoop *mLoop;											// EventLoop指针
	Socket *mSocket;											// Socket指针
	Channel *mChannel { nullptr };								// Channel指针
	Buffer *mReadBuffer{ nullptr };								// 读缓冲区
	Buffer *mSendBuffer{ nullptr };								// 写缓冲区
	State mState { Invalid };									// 连接状态
	std::function<void(Socket *)> mDeleteConnectionCallback;	// 删除连接的回调函数
	std::function<void(Connection *)> mOnConnectCallback;		// 业务逻辑回调函数

	void readNonBlocking();		// 非阻塞读
	void writeNonBlocking();	// 非阻塞写
	void readBlocking();		// 阻塞读
	void writeBlocking();		// 阻塞写

public:
	explicit Connection(EventLoop *loop, Socket *sock);
	~Connection();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Connection);

	/**
	 * @brief 读操作
	 */
	void read();

	/**
	 * @brief 写操作
	 */
	void write();

	/**
	 * @brief 设置连接时的业务逻辑回调函数
	 *
	 * @param callback 回调函数
	 */
	void setOnConnectionCallback(std::function<void(Connection *)> const &callback);

	/**
	 * @brief 设置删除连接时要调用的回调函数
	 *
	 * @param _callback 删除连接时要调用的回调函数
	 */
	void setDeleteConnectionCallback(std::function<void(Socket *)> const &callback);

	/**
	 * @brief 获取当前连接状态
	 *
	 * @return 当前状态
	 */
	State getState();

	/**
	 * @brief 关闭连接
	 */
	void close();

	/**
	 * @brief 设置写缓冲区
	 *
	 * @param str 字符串
	 */
	void setSentBuffer(char const *str);

	/**
	 * @brief 获取读缓冲区
	 *
	 * @return 读缓冲区
	 */
	Buffer *getReadBuffer();

	/**
	 * @brief 获取读缓冲区内容
	 *
	 * @return 读缓冲区内容
	 */
	char const *readBuffer();

	/**
	 * @brief 获取写缓冲区
	 *
	 * @return 写缓冲区
	 */
	Buffer *getSendBuffer();

	/**
	 * @brief 获取写缓冲区内容
	 *
	 * @return 写缓冲区内容
	 */
	char const *sendBuffer();

	/**
	 * @brief 获取一行写入写缓冲区
	 */
	void getlineSendBuffer();

	/**
	 * @brief 获取连接的Socket
	 *
	 * @return Socket
	 */
	Socket *getSocket();

	/**
	 * @brief 设置连接的业务逻辑
	 *
	 * @param fn 业务逻辑函数
	 */
	void onConnect(std::function<void()> fn);
};
