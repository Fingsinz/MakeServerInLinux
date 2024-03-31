#pragma once

#include <vector>

class Channel;
class EventLoop;
class Socket;
class Server;
class InetAddress;

class Server
{
private:
	// ָ��EventLoop�����ָ��
	EventLoop *loop;

	// �������׽���
	Socket *serverSock;

	// ��������ַ
	InetAddress *serverAddr;

	// ������ͨ��
	Channel *serverChannel;

	// ����ͻ��˵��׽���
	std::vector<std::pair<Socket *, InetAddress *>> clients;

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
