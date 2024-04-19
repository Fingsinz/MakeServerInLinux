#pragma once

#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor
{
private:
	// �����¼������EventLoopָ��
	EventLoop *loop;

	// ���ڴ����׽��ֲ������׽���ָ��
	Socket *sock;

	// ���ڴ洢��ַ��Ϣ��ָ��
	InetAddress *addr;

	// ���ڽ������ӵ� Channel ָ��
	Channel *acceptChannel;

	// ����һ���½����ӵĻص�����
	std::function<void(Socket *)> newConnectionCallback;

public:
	Acceptor(EventLoop *_loop);
	~Acceptor();

	/**
	 * @brief ����������
	 */
	void acceptConnection();

	/**
	 * @brief ���������ӵĻص�������
	 *
	 * @param _callback Ϊ���������õĻص�������
	 */
	void setNewConnectionCallback(std::function<void(Socket *)> _callback);
};
