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
	 * @brief 将文件描述符添加到具有指定事件类型的epoll实例
	 *
	 * @param fd 要添加的文件描述符
	 * @param op 要与文件描述符关联的事件类型
	 */
	void addFd(int fd, uint32_t op);

	/**
	 * @brief 更新 Channel 信息
	 *
	 * @param channel 指向要更新的 Channel 的指针
	 */
	void updateChannel(Channel *channel);

	/**
	 * @brief 用给定的超时轮询epoll事件
	 *
	 * @param timeout 超时值，以毫秒为单位。默认值是-1
	 * @return Channel 实例的向量
	 */
	vector<Channel *> poll(int timeout = -1);
};
