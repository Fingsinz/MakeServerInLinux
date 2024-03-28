#pragma once

#include <sys/epoll.h>
#include <vector>

using std::vector;

class Epoll
{
private:
	int epfd;
	struct epoll_event *events;

public:
	Epoll();
	~Epoll();

	/*
	* ���ļ���������ӵ�����ָ���¼����͵�epollʵ��
	* @param fd Ҫ��ӵ��ļ�������
	* @param op Ҫ���ļ��������������¼�����
	*/
	void addFd(int fd, uint32_t op);

	/*
	* �ø����ĳ�ʱ��ѯepoll�¼�
	* @param timeout ��ʱֵ���Ժ���Ϊ��λ��Ĭ��ֵ��-1(����)
	* @return epoll�¼�������
	*/
	vector<epoll_event> poll(int timeout = -1);
};
