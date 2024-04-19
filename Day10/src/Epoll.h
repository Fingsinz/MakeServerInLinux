#pragma once

#include <sys/epoll.h>
#include <vector>

using std::vector;

class Channel;
class Epoll
{
private:
	// epoll�ļ�������
	int epfd;

	// epoll�¼�����
	struct epoll_event *events;

public:
	Epoll();
	~Epoll();

	/**
	 * @brief ���� Channel ��Ϣ
	 *
	 * @param channel ָ��Ҫ���µ� Channel ��ָ��
	 */
	void updateChannel(Channel *channel);

	void deleteChannel(Channel *channel);

	/**
	 * @brief �ø����ĳ�ʱ��ѯepoll�¼�
	 *
	 * @param timeout ��ʱֵ���Ժ���Ϊ��λ��Ĭ��ֵ��-1
	 * @return Channel ʵ��������
	 */
	vector<Channel *> poll(int timeout = -1);
};
