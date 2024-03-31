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
	 * @brief ���ļ���������ӵ�����ָ���¼����͵�epollʵ��
	 *
	 * @param fd Ҫ��ӵ��ļ�������
	 * @param op Ҫ���ļ��������������¼�����
	 */
	void addFd(int fd, uint32_t op);

	/**
	 * @brief ���� Channel ��Ϣ
	 *
	 * @param channel ָ��Ҫ���µ� Channel ��ָ��
	 */
	void updateChannel(Channel *channel);

	/**
	 * @brief �ø����ĳ�ʱ��ѯepoll�¼�
	 *
	 * @param timeout ��ʱֵ���Ժ���Ϊ��λ��Ĭ��ֵ��-1
	 * @return Channel ʵ��������
	 */
	vector<Channel *> poll(int timeout = -1);
};
