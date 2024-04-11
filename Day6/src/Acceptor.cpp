#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
	// ����һ��Socket�������õ�ַ
	sock = new Socket();
	addr = new InetAddress("127.0.0.1", 1234);
	sock->bind(addr);

	// ������������Ӳ����׽�������Ϊ������ģʽ
	sock->listen();
	sock->setNonBlocking();

	// ����һ���������ӵ���Channel
	acceptChannel = new Channel(loop, sock->getFd());

	// ����һ�����ڽ���Channel�Ļص�������������
	std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
	acceptChannel->setCallback(cb);

	// �ڽ���ͨ�������ö��¼�
	acceptChannel->enableReading();
}

Acceptor::~Acceptor()
{
	delete sock;
	delete addr;
	delete acceptChannel;
}

void Acceptor::acceptConnection()
{
	// �½�����ʱ�����ûص�����
	newConnectionCallback(sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _callback)
{
	// �����µ����ӻص�����
	newConnectionCallback = _callback;
}