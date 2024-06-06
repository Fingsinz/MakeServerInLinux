#pragma once

#include "Common.h"
#include <functional>
#include <memory>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection
{
public:
	enum State
	{
		Invalid = 0, // 初始无效状态
		Handshaking, // 握手过程中的状态
		Connected,   // 连接建立
		Closed,      // 连接关闭
		Failed,      // 连接失败
	};

private:
	//EventLoop *mLoop;											// EventLoop指针
	std::unique_ptr<Socket> mSocket;							// Socket指针
	std::unique_ptr<Channel> mChannel{ nullptr };				// Channel指针
	std::unique_ptr<Buffer> mReadBuffer{ nullptr };				// 读缓冲区
	std::unique_ptr<Buffer> mSendBuffer{ nullptr };				// 写缓冲区

	State mState{ Invalid };									// 连接状态
	std::function<void(int)> mDeleteConnectionCallback;			// 删除连接的回调函数
	std::function<void(Connection *)> mOnMessageCallback;		// 业务逻辑回调函数

	FLAG readNonBlocking();		// 非阻塞读
	FLAG writeNonBlocking();	// 非阻塞写
	FLAG readBlocking();		// 阻塞读
	FLAG writeBlocking();		// 阻塞写

public:
	explicit Connection(EventLoop *loop, int fd);
	~Connection();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Connection);

	/**
	 * @brief 从与连接相关联的套接字读取数据
	 * 
	 * @return FLAG 操作结果标记
	 */
	FLAG read();

	/**
	 * @brief 将数据写入与该连接相关联的套接字
	 * 
	 * @return FLAG 操作结果标记
	 */
	FLAG write();

	/**
	 * @brief 通过连接发送消息
	 * 
	 * @param msg 要发送的消息
	 * @return FLAG 操作结果标记
	 */
	FLAG send(std::string msg);

	/**
	 * @brief 设置一个回调函数，在连接上接收到消息时调用
	 * 
	 * @param callback 回调函数
	 */
	void setOnMessageCallback(std::function<void(Connection *)> const &callback);

	/**
	 * @brief 设置一个回调函数，在需要删除连接时调用
	 * 
	 * @param callback 回调函数
	 */
	void setDeleteConnectionCallback(std::function<void(int)> const &callback);

	/**
	 * @brief 与连接关联的业务逻辑
	 */
	void business();

	/**
	 * @brief 获取连接的状态
	 * 
	 * @return 状态
	 */
	State getState();

	/**
	 * @brief 关闭这个连接
	 */
	void close();

	/**
	 * @brief 设置要通过连接发送的缓冲区
	 * 
	 * @param str 要发送的缓冲区
	 */
	void setSentBuffer(char const *str);

	/**
	 * @brief 获取与该连接关联的读缓冲区
	 * 
	 * @return 指向读缓冲区的指针
	 */
	Buffer *getReadBuffer();

	/**
	 * @brief 获取与该连接关联的发送缓冲区
	 * 
	 * @return 指向写缓冲区的指针
	 */
	Buffer *getSendBuffer();

	/**
	 * @brief 获取与该连接关联的套接字
	 * 
	 * @return Socket指针
	 */
Socket *getSocket();
};
