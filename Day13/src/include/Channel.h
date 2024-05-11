#pragma once

#include "Macros.h"
#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
	EventLoop *mLoop;					// 指向与之关联的事件循环
	Socket *mSocket;					// 与之关联的Socket
	uint32_t mListenEvents{ 0 };		// 监听的事件
	uint32_t mReadyEvents{ 0 };			// 就绪事件
	bool exist{ false };				// 指示该Channel是否存在有效
	std::function<void()> readCallback;	// 读回调
	std::function<void()> writeCallback;// 写回调

public:
	explicit Channel(EventLoop *loop, Socket *socket);
	~Channel();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Channel);

	/**
	 * @brief 处理事件
	 */
	void handleEvent();

	/**
	 * @brief 开启读操作
	 */
	void enableRead();

	/**
	 * @brief 开启写操作
	 */
	void enableWrite();

	/**
	 * @brief 使用ET
	 */
	void useET();

	/**
	 * @brief 获取Socket
	 *
	 * @return 返回Socket指针
	 */
	Socket *getSocket() const;

	/**
	 * @brief 获取监听事件
	 *
	 * @return 监听事件
	 */
	uint32_t getListenEvents();

	/**
	 * @brief 获取就绪事件
	 *
	 * @return 就绪事件
	 */
	uint32_t getReadyEvents();

	/**
	 * @brief 设置就绪事件
	 *
	 * @param 就绪事件
	 */
	void setReadyEvents(uint32_t events);

	/**
	 * @brief 检查有效性
	 *
	 * @return 如果存在则为true，否则为false
	 */
	bool getExist() const;

	/**
	 * @brief 设置有效性
	 *
	 * @param _exist 如果存在为真，否则为假
	 */
	void setExist(bool _exist);

	/**
	 * @brief 设置回调函数。
	 *
	 * @param _callback要设置的回调函数。
	 */
	void setReadCallback(std::function<void()> const &callback);
};
