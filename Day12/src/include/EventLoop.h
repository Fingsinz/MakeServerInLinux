#pragma once

#include <functional>

class Epoll;
class Channel;

class EventLoop
{
private:
	Epoll *ep{ nullptr };	// 指向Epoll类实例的指针
	bool quit{ false };		// 指示程序是否应该退出

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
