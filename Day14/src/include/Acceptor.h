#pragma once

#include "Common.h"
#include <memory>
#include <functional>

class Acceptor
{
private:
	std::unique_ptr<Socket> mSocket;	// 用于处理套接字操作的套接字指针
	std::unique_ptr<Channel> mChannel;	// 用于接受连接的 Channel 指针

	// 定义一个新建连接的回调函数
	std::function<void(int)> mNewConnectionCallback;

public:

	// 禁用拷贝和移动
	DISALLOW_COPY_AND_MOVE(Acceptor);

	explicit Acceptor(EventLoop *loop);
	~Acceptor();

	/**
	 * @brief 接受连接
	 *
	 * @return 连接的情况
	 */
	FLAG acceptConnection() const;

	/**
	 * @brief 设置新连接的回调函数。
	 *
	 * @param callback 为新连接设置的回调函数。
	 */
	void setNewConnectionCallback(std::function<void(int)> const &callback);
};
