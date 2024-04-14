#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
private:
	// 线程池中的线程
	std::vector<std::thread> threads;

	// 要执行的函数
	std::queue<std::function<void()>> tasks;

	// 声明互斥锁以同步对任务队列的访问
	std::mutex tasksMtx;

	// 声明在线程之间进行协调的条件变量
	std::condition_variable cv;

	// 指示线程停止的标志
	bool stop;

public:
	ThreadPool(int size = 10);
	~ThreadPool();

	/**
	 * @brief 加入任务到任务队列中
	 *
	 * @param task 要被加入的任务
	 */
	void add(std::function<void()> task);
};
