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
	// �̳߳��е��߳�
	std::vector<std::thread> threads;

	// Ҫִ�еĺ���
	std::queue<std::function<void()>> tasks;

	// ������������ͬ����������еķ���
	std::mutex tasksMtx;

	// �������߳�֮�����Э������������
	std::condition_variable cv;

	// ָʾ�߳�ֹͣ�ı�־
	bool stop;

public:
	ThreadPool(int size = 10);
	~ThreadPool();

	/**
	 * @brief ��������ӵ��̳߳ز����ؽ����future��
	 *
	 * @tparam F Ҫִ�еĺ��������͡�
	 * @tparam Args ���ݸ������Ĳ������͡�
	 * @param f Ҫִ�еĺ�����
	 * @param args ���ݸ������Ĳ�����
	 * @return std::future<typename std::result_of<F(Args...)>::type> ���������future
	 */
	template <class F, class... Args>
	auto add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
	// ʹ��std:��result_ofΪ����ΪArgs���ĺ���F�ķ������ʹ�������returnType��
	using returnType = typename std::result_of<F(Args ...)>::type;

	// ����һ��shared_ptrָ��packaged_task����packaged_task�����װ�����������
	auto task = std::make_shared<std::packaged_task<returnType()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	// ��ȡ��packaged_task������future����
	std::future<returnType> res = task->get_future();

	{
		std::unique_lock<std::mutex> lock(tasksMtx);
		if (stop)	// �׳��쳣
			throw std::runtime_error("ThreadPool Stopped");

		// ��������ӵ����������
		tasks.emplace([task] () {(*task)(); });
	}

	// ֪ͨһ���ȴ��߳���һ�����������
	cv.notify_one();

	// ����future����
	return res;
}