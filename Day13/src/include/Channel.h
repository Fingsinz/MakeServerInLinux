#pragma once

#include "Macros.h"
#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
	EventLoop *mLoop;			// 指向与之关联的事件循环的指针
	int mFd;					// 与之关联的文件描述符
	uint32_t mEvents { 0 };		// 希望监听的事件
	uint32_t mReady { 0 };		// 用于存储就绪事件
	bool inEpoll;				// 指示文件描述符是否在epoll集合中

	// 发生事件时执行的回调函数
	std::function<void()> readCallback;
	std::function<void()> writeCallback;

public:
	explicit Channel(EventLoop *loop, int fd);
	~Channel();

	// 禁止拷贝和移动
	DISALLOW_COPY_AND_MOVE(Channel);

	/**
	 * @brief 处理事件
	 */
	void handleEvent();

	/**
	 * @brief 启动监听
	 */
	void enableReading();

	/**
	 * @brief 获取文件描述符
	 *
	 * @return int 类型，文件描述符
	 */
	int getFd();

	/**
	 * @brief 获取希望监听事件
	 *
	 * @return 事件
	 */
	uint32_t getEvents();

	/**
	 * @brief 返回 ready值
	 *
	 * @return uint32_t ready值
	 */
	uint32_t getReady();

	/**
	 * @brief 检查文件描述符是否在epoll集合中
	 *
	 * @return 如果文件描述符在epoll集合中，则为True，否则为false
	 */
	bool getInEpoll();

	/**
	 * @brief 设置对象是否处于Epoll中。
	 *
	 * @param in 布尔值
	 */
	void setInEpoll(bool in = true);

	/**
	 * @brief 使用 ET 模式
	 */
	void useET();

	/**
	 * @brief 设置Ready值
	 *
	 * @param ready ready值。
	 */
	void setReady(uint32_t ready);

	/**
	 * @brief 设置回调函数。
	 *
	 * @param _callback要设置的回调函数。
	 */
	void setReadCallback(std::function<void()> const &callback);
};
