#pragma once

#include "Epoll.h"
#include "Channel.h"

class EventLoop
{
private:
	// ָ��Epoll��ʵ����ָ��
	Epoll *ep;

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
};
