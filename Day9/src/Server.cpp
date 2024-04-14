#include "Acceptor.h"
#include "Server.h"
#include "Socket.h"
#include "Connection.h"
#include <functional>

constexpr int READ_BUFFER = 1024;

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr)
{
	// ʹ�ø������¼�ѭ������һ���µ�Acceptor����
	acceptor = new Acceptor(loop);

	// ����һ���ص�����cb��������һ��Socketָ����Ϊ����
	std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);

	// ��acceptor�������ӻص�����Ϊ�Ѷ���Ļص�����
	acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
	delete acceptor;
}

void Server::handleReadEvent(int fd)
{}

void Server::newConnection(Socket *_socket)
{
	Connection *conn = new Connection(loop, _socket);
	std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
	conn->setDeleteConnectionCallback(cb);
	connections[_socket->getFd()] = conn;
}

void Server::deleteConnection(Socket *_socket)
{
	Connection *conn = connections[_socket->getFd()];
	connections.erase(_socket->getFd());
	delete conn;
}