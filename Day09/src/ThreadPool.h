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
	 * @brief �����������������
	 *
	 * @param task Ҫ�����������
	 */
	void add(std::function<void()> task);
};
