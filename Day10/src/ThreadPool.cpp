#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false)
{
	// �����߳�
	for (int i = 0; i < size; ++ i)
	{
		threads.emplace_back(std::thread([this] ()
			{
				while (true)
				{
					std::function<void()> task;

					{
						// ʹ�������������ڽ��� std::mutex����������unlock()
						std::unique_lock<std::mutex> lock(tasksMtx);

						// ��������в�Ϊ�ջ��̳߳�ֹͣʱֹͣ�ȴ���������
						cv.wait(lock, [this] ()
							{
								return stop or !tasks.empty();
							});
						if (stop and tasks.empty())	// �������Ϊ�����̳߳�ֹͣ���˳�ѭ��
							return;

						task = tasks.front();
						tasks.pop();
					}

					task();	// ִ������
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

	// ֪ͨ���еȴ����߳��̳߳�����ֹͣ
	cv.notify_all();

	for (std::thread &th : threads)
	{
		// ����߳��Ƿ�ɽӺ�
		if (th.joinable())
			th.join();	// ������ǰ�̣߳�ֱ��ָ���߳������ִ��
	}
}