#include "InetAddress.h"
#include "Socket.h"
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

Socket::Socket() : fd(-1)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	errorif(fd == -1, "Socket create error");
}

Socket::Socket(int _fd) : fd(_fd)
{
	errorif(fd == -1, "Socket create error");
}

Socket::~Socket()
{
	if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
}

void Socket::bind(InetAddress *_addr)
{
	// �洢��ַ��Ϣ
	struct sockaddr_in addr = _addr->getAddr();
	socklen_t addrLen = _addr->getAddrLen();

	// ���׽��ְ󶨵��������ĵ�ַ��������Ƿ��д���
	errorif(::bind(fd, (sockaddr *)&addr, addrLen) == -1, "Socket bind error");

	// ʹ�ð󶨵ĵ�ַ�ͳ����������õ�ַ��Ϣ
	_addr->setInetAddr(addr, addrLen);
}

void Socket::listen()
{
	errorif(::listen(fd, SOMAXCONN) == -1, "Socket listen error");
}

void Socket::setNonBlocking()
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *_addr)
{
	//����һ��sockaddr_in�ṹ������ͻ��˵�ַ��Ϣ
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	bzero(&addr, sizeof(addr));

	// �����׽����ϵ����ӣ����ͻ�����Ϣ�洢��addr��addrLen��
	int clientSock = ::accept(fd, (sockaddr *)&addr, &addrLen);
	errorif(clientSock == -1, "Socket accept error");

	// ʹ�ý��ܵĵ�ַ�ͳ������ÿͻ��˵�ַ��Ϣ
	_addr->setInetAddr(addr, addrLen);

	return clientSock;
}

void Socket::connect(InetAddress *_addr)
{
	struct sockaddr_in addr = _addr->getAddr();
	socklen_t addrLen = _addr->getAddrLen();
	errorif(::connect(fd, (sockaddr *)&addr, addrLen) == -1, "Socket connect error");
}

int Socket::getFd()
{
	return fd;
}