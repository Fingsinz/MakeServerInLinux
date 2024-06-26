#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

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
	 * @brief 将任务添加到线程池并返回结果的future。
	 *
	 * @tparam F 要执行的函数的类型。
	 * @tparam Args 传递给函数的参数类型。
	 * @param f 要执行的函数。
	 * @param args 传递给函数的参数。
	 * @return std::future<typename std::result_of<F(Args...)>::type> 函数结果的future
	 */
	template <class F, class... Args>
	auto add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	// 使用std:：result_of为参数为Args…的函数F的返回类型创建别名returnType。
	using returnType = typename std::result_of<F(Args ...)>::type;

	// 创建一个shared_ptr指向packaged_task对象，packaged_task对象包装函数及其参数
	auto task = std::make_shared<std::packaged_task<returnType()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	// 获取与packaged_task关联的future对象
	std::future<returnType> res = task->get_future();

	{
		std::unique_lock<std::mutex> lock(tasksMtx);
		if (stop)	// 抛出异常
			throw std::runtime_error("ThreadPool Stopped");

		// 将任务添加到任务队列中
		tasks.emplace([task] () {(*task)(); });
	}

	// 通知一个等待线程有一个新任务可用
	cv.notify_one();

	// 返回future对象
	return res;
}