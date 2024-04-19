#pragma once

#include <map>

class Acceptor;
class EventLoop;
class Connection;
class Socket;

class Server
{
private:
	// ָ��EventLoop�����ָ��
	EventLoop *loop;

	// ָ��Acceptor�����ָ��
	Acceptor *acceptor;

	// �洢���Ӽ�����Ӧ���ļ�������
	std::map<int, Connection *> connections;

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

	/**
	 * @brief �Ͽ����ṩ���׽��ֹ���������
	 *
	 * @param _sockfd ��Ҫɾ�������ӵ��׽����ļ�������
	 */
	void deleteConnection(int sockfd);
};
