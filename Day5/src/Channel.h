#pragma once

#include <sys/epoll.h>
#include <functional>
#include "EventLoop.h"

class Channel
{
private:
	// 指向与之关联的事件循环的指针
	EventLoop *loop;

	// 与之关联的文件描述符
	int fd;

	// 希望监听的事件
	uint32_t events;

	// 正发生的事件
	uint32_t revents;

	// 指示文件描述符是否在epoll集合中
	bool inEpoll;

	// 发生事件时执行的回调函数
	std::function<void()> callback;

public:
	Channel(EventLoop *_loop, int _fd);
	~Channel();

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
	 * @brief 获取正在发生的事件
	 *
	 * @return uint32_t 事件
	 */
	uint32_t getRevents();

	/**
	 * @brief 检查文件描述符是否在epoll集合中
	 *
	 * @return 如果文件描述符在epoll集合中，则为True，否则为false
	 */
	bool getInEpoll();

	/**
	 * 设置epoll事件监控的文件描述符。
	 */
	void setInEpoll();

	/**
	 * @brief 设置已发生的事件。
	 *
	 * @param 已发生的事件。
	 */
	void setRevents(uint32_t _revents);

	/**
	 * @brief 设置回调函数。
	 *
	 * @param _callback要设置的回调函数。
	 */
	void setCallback(std::function<void()> _callback);
};
