#pragma once

#include "Common.h"

#ifdef  OS_LINUX
#include <sys/epoll.h>
#endif //  OS_LINUX

#include <vector>

class Epoll
{
private:
	int mEpFd{ -1 };						// epoll文件描述符
	struct epoll_event *mEvents{ nullptr };	// epoll事件

public:

	// 禁用拷贝和移动
	DISALLOW_COPY_AND_MOVE(Epoll);

	Epoll();
	~Epoll();

	FLAG updateChannel(Channel *channel) const;
	FLAG deleteChannel(Channel *channel) const;

	std::vector<Channel *> poll(int timeout = -1) const;
};
