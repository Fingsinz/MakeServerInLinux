#pragma once

#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
	// ָ����֮�������¼�ѭ����ָ��
	EventLoop *loop;

	// ��֮�������ļ�������
	int fd;

	// ϣ���������¼�
	uint32_t events;

	// ���ڴ洢�����¼�
	uint32_t ready;

	// ָʾ�ļ��������Ƿ���epoll������
	bool inEpoll;

	// �Ƿ�ʹ���̳߳�
	bool useThreadPool;

	// �����¼�ʱִ�еĻص�����
	std::function<void()> readCallback;
	std::function<void()> writeCallback;

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
	 * @brief ���� readyֵ
	 *
	 * @return uint32_t readyֵ
	 */
	uint32_t getReady();

	/**
	 * @brief ����ļ��������Ƿ���epoll������
	 *
	 * @return ����ļ���������epoll�����У���ΪTrue������Ϊfalse
	 */
	bool getInEpoll();

	/**
	 * @brief ���ö����Ƿ���Epoll�С�
	 *
	 * @param _in ����ֵ
	 */
	void setInEpoll(bool _in = true);

	/**
	 * @brief ʹ�� ET ģʽ
	 */
	void useET();

	/**
	 * @brief ����Readyֵ
	 *
	 * @param readyֵ��
	 */
	void setReady(uint32_t _ready);

	/**
	 * @brief ���ûص�������
	 *
	 * @param _callbackҪ���õĻص�������
	 */
	void setReadCallback(std::function<void()> _callback);

	/**
	 * @brief �����Ƿ�ʹ���̳߳�
	 *
	 * @param use ����ֵ��Ĭ��ʹ��
	 */
	void setUseThreadPool(bool use = true);
};
