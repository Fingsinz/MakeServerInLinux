#pragma once

#include <sys/epoll.h>
#include <vector>

using std::vector;

class Channel;
class Epoll
{
private:
	int epfd{ -1 };					// epoll文件描述符
	struct epoll_event *events{ nullptr };	// epoll事件数组

public:
	Epoll();
	~Epoll();

	/**
	 * @brief 更新 Channel 信息
	 *
	 * @param channel 指向要更新的 Channel 的指针
	 */
	void updateChannel(Channel *channel);

	/**
	 * @brief 删除Channel
	 *
	 * @param channel 要删除的Channel指针
	 */
	void deleteChannel(Channel *channel);

	/**
	 * @brief 用给定的超时轮询epoll事件
	 *
	 * @param timeout 超时值，以毫秒为单位。默认值是-1
	 * @return Channel 实例的向量
	 */
	vector<Channel *> poll(int timeout = -1);
};
