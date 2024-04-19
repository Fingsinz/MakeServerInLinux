#pragma once

#include <sys/epoll.h>
#include <vector>

using std::vector;

class Channel;
class Epoll
{
private:
	// epoll文件描述符
	int epfd;

	// epoll事件数组
	struct epoll_event *events;

public:
	Epoll();
	~Epoll();

	/**
	 * @brief 更新 Channel 信息
	 *
	 * @param channel 指向要更新的 Channel 的指针
	 */
	void updateChannel(Channel *channel);

	void deleteChannel(Channel *channel);

	/**
	 * @brief 用给定的超时轮询epoll事件
	 *
	 * @param timeout 超时值，以毫秒为单位。默认值是-1
	 * @return Channel 实例的向量
	 */
	vector<Channel *> poll(int timeout = -1);
};
