#pragma once

#include "Epoll.h"
#include "Channel.h"

class EventLoop
{
private:
	// 指向Epoll类实例的指针
	Epoll *ep;

	// 指示程序是否应该退出
	bool quit;

public:
	EventLoop();
	~EventLoop();

	/**
	 * @brief 循环函数
	 */
	void loop();

	/**
	 * @brief 更新 Channel
	 *
	 * @param channel 指向要更新的Channel对象的指针
	 */
	void updateChannel(Channel *channel);
};
