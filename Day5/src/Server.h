#pragma once

#include "EventLoop.h"
#include "Socket.h"

class Server
{
private:
	// ָ��EventLoop�����ָ��
	EventLoop *loop;
	// ����ͻ����ڴ�
	vector<std::pair<InetAddress *, Socket *>> clients;

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
