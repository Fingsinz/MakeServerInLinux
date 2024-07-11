#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"
#include <iostream>

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
	delete addr;
}

Acceptor::~Acceptor()
{
	delete sock;
	delete acceptChannel;
}

void Acceptor::acceptConnection()
{
	// ����һ���µ�InetAddress�������洢�ͻ��˵�ַ��Ϣ
	InetAddress *clientAddr = new InetAddress();

	//ͨ��ʹ�ÿͻ��˵�ַ�������Է������׽��ֵ����ӣ�����һ���µ�Socket����
	Socket *clientSock = new Socket(sock->accept(clientAddr));

	// ��ӡ�й��¿ͻ������ӵ���Ϣ
	std::cout << "[New client " << clientSock->getFd() << " ]:\t(" <<
		inet_ntoa(clientAddr->getAddr().sin_addr) << " : " << ntohs(clientAddr->getAddr().sin_port) << ")\n";

	clientSock->setNonBlocking();

	newConnectionCallback(clientSock);
	delete clientAddr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _callback)
{
	// �����µ����ӻص�����
	newConnectionCallback = _callback;
}