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
	* 将文件描述符添加到具有指定事件类型的epoll实例
	* @param fd 要添加的文件描述符
	* @param op 要与文件描述符关联的事件类型
	*/
	void addFd(int fd, uint32_t op);

	/*
	* 用给定的超时轮询epoll事件
	* @param timeout 超时值，以毫秒为单位。默认值是-1(无限)
	* @return epoll事件的向量
	*/
	vector<epoll_event> poll(int timeout = -1);
};
