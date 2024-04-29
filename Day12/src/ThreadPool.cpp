#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false)
{
	// 启动线程
	for (int i = 0; i < size; ++ i)
	{
		threads.emplace_back(std::thread([this] ()
			{
				while (true)
				{
					std::function<void()> task;

					{
						// 使用作用域生命期解锁 std::mutex，而不调用unlock()
						std::unique_lock<std::mutex> lock(tasksMtx);

						// 当任务队列不为空或线程池停止时停止等待（阻塞）
						cv.wait(lock, [this] ()
							{
								return stop or !tasks.empty();
							});
						if (stop and tasks.empty())	// 任务队列为空且线程池停止，退出循环
							return;

						task = tasks.front();
						tasks.pop();
					}

					task();	// 执行任务
				}
			}));
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(tasksMtx);
		stop = true;
	}

	// 通知所有等待的线程线程池正在停止
	cv.notify_all();

	for (std::thread &th : threads)
	{
		// 检查线程是否可接合
		if (th.joinable())
			th.join();	// 阻塞当前线程，直到指定线程完成其执行
	}
}