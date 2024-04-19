#pragma once

#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
	// 指向与之关联的事件循环的指针
	EventLoop *loop;

	// 与之关联的文件描述符
	int fd;

	// 希望监听的事件
	uint32_t events;

	// 用于存储就绪事件
	uint32_t ready;

	// 指示文件描述符是否在epoll集合中
	bool inEpoll;

	// 是否使用线程池
	bool useThreadPool;

	// 发生事件时执行的回调函数
	std::function<void()> readCallback;
	std::function<void()> writeCallback;

public:
	Channel(EventLoop *_loop, int _fd);
	~Channel();

	/**
	 * @brief 处理事件
	 */
	void handleEvent();

	/**
	 * @brief 启动监听
	 */
	void enableReading();

	/**
	 * @brief 获取文件描述符
	 *
	 * @return int 类型，文件描述符
	 */
	int getFd();

	/**
	 * @brief 获取希望监听事件
	 *
	 * @return 事件
	 */
	uint32_t getEvents();

	/**
	 * @brief 返回 ready值
	 *
	 * @return uint32_t ready值
	 */
	uint32_t getReady();

	/**
	 * @brief 检查文件描述符是否在epoll集合中
	 *
	 * @return 如果文件描述符在epoll集合中，则为True，否则为false
	 */
	bool getInEpoll();

	/**
	 * @brief 设置对象是否处于Epoll中。
	 *
	 * @param _in 布尔值
	 */
	void setInEpoll(bool _in = true);

	/**
	 * @brief 使用 ET 模式
	 */
	void useET();

	/**
	 * @brief 设置Ready值
	 *
	 * @param ready值。
	 */
	void setReady(uint32_t _ready);

	/**
	 * @brief 设置回调函数。
	 *
	 * @param _callback要设置的回调函数。
	 */
	void setReadCallback(std::function<void()> _callback);

	/**
	 * @brief 设置是否使用线程池
	 *
	 * @param use 布尔值，默认使用
	 */
	void setUseThreadPool(bool use = true);
};
