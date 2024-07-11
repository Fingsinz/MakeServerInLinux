#pragma once

#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop
{
private:
	// ָ��Epoll��ʵ����ָ��
	Epoll *ep;

	// �̳߳�ָ��
	ThreadPool *threadPool;

	// ָʾ�����Ƿ�Ӧ���˳�
	bool quit;

public:
	EventLoop();
	~EventLoop();

	/**
	 * @brief ѭ������
	 */
	void loop();

	/**
	 * @brief ���� Channel
	 *
	 * @param channel ָ��Ҫ���µ�Channel�����ָ��
	 */
	void updateChannel(Channel *channel);

	/**
	 * @brief ʹ�ø����������̳߳����һ�����̡߳�
	 * @param task ���߳�Ҫִ�е�����
	 */
	void addThread(std::function<void()> task);
};
