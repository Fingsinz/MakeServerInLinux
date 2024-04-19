#pragma once

#include <functional>

class Buffer;
class EventLoop;
class Socket;
class Channel;

class Connection
{
private:

	// EventLoopָ��
	EventLoop *loop;
	// Socketָ��
	Socket *sock;
	// Channelָ��
	Channel *channel;
	// ɾ�����ӵĻص�����
	std::function<void(int)> deleteConnectionCallback;
	// ��������
	Buffer *readBuffer;

public:
	Connection(EventLoop *_loop, Socket *_sock);
	~Connection();

	/**
	 * @brief ����sockfd����������
	 *
	 * @param sockfd ͨ���׽����ļ���������������
	 */
	void echo(int sockfd);

	/**
	 * @brief ����ɾ������ʱҪ���õĻص�����
	 *
	 * @param _callback ɾ������ʱҪ���õĻص�����
	 */
	void setDeleteConnectionCallback(std::function<void(int)> _callback);

	/**
	 * @brief ͨ��ָ�����׽��ַ������ݡ�
	 *
	 * @param sockfd �׽����ļ�������
	 */
	void send(int sockfd);
};
