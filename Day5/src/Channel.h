#pragma once

#include <sys/epoll.h>
#include <functional>
#include "EventLoop.h"

class Channel
{
private:
	// ָ����֮�������¼�ѭ����ָ��
	EventLoop *loop;

	// ��֮�������ļ�������
	int fd;

	// ϣ���������¼�
	uint32_t events;

	// ���������¼�
	uint32_t revents;

	// ָʾ�ļ��������Ƿ���epoll������
	bool inEpoll;

	// �����¼�ʱִ�еĻص�����
	std::function<void()> callback;

public:
	Channel(EventLoop *_loop, int _fd);
	~Channel();

	/**
	 * @brief �����¼�
	 */
	void handleEvent();

	/**
	 * @brief ��������
	 */
	void enableReading();

	/**
	 * @brief ��ȡ�ļ�������
	 *
	 * @return int ���ͣ��ļ�������
	 */
	int getFd();

	/**
	 * @brief ��ȡϣ�������¼�
	 *
	 * @return �¼�
	 */
	uint32_t getEvents();

	/**
	 * @brief ��ȡ���ڷ������¼�
	 *
	 * @return uint32_t �¼�
	 */
	uint32_t getRevents();

	/**
	 * @brief ����ļ��������Ƿ���epoll������
	 *
	 * @return ����ļ���������epoll�����У���ΪTrue������Ϊfalse
	 */
	bool getInEpoll();

	/**
	 * ����epoll�¼���ص��ļ���������
	 */
	void setInEpoll();

	/**
	 * @brief �����ѷ������¼���
	 *
	 * @param �ѷ������¼���
	 */
	void setRevents(uint32_t _revents);

	/**
	 * @brief ���ûص�������
	 *
	 * @param _callbackҪ���õĻص�������
	 */
	void setCallback(std::function<void()> _callback);
};
