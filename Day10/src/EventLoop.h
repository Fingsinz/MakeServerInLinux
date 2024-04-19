#pragma once

#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop
{
private:
	// 指向Epoll类实例的指针
	Epoll *ep;

	// 线程池指针
	ThreadPool *threadPool;

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

	/**
	 * @brief 使用给定任务向线程池添加一个新线程。
	 * @param task 新线程要执行的任务
	 */
	void addThread(std::function<void()> task);
};
