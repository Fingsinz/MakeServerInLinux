#pragma once

#include <vector>

class Acceptor;
class EventLoop;
class Socket;

class Server
{
private:
	// ָ��EventLoop�����ָ��
	EventLoop *loop;

	// ָ��Acceptor�����ָ��
	Acceptor *acceptor;

public:
	Server(EventLoop *_loop);
	~Server();

	/**
	 * @brief ��������ļ��������Ķ�ȡ�¼�
	 *
	 * @param fd Ҫ��ȡ���ļ�����
	 */
	void handleReadEvent(int fd);

	/**
	 * @brief ���������ṩ�׽��ֵ�������
	 *
	 * @param _socket ��ʾ�����ӵ��׽���
	 */
	void newConnection(Socket *_socket);
};
