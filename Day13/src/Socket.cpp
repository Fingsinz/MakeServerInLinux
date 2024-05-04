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
	// 存储地址信息
	struct sockaddr_in addr = _addr->getAddr();
	socklen_t addrLen = _addr->getAddrLen();

	// 将套接字绑定到检索到的地址，并检查是否有错误
	errorif(::bind(fd, (sockaddr *)&addr, addrLen) == -1, "Socket bind error");

	// 使用绑定的地址和长度重新设置地址信息
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

bool Socket::isNonBlocking()
{
	return (fcntl(fd, F_GETFL) & O_NONBLOCK) != 0;
}

int Socket::accept(InetAddress *_addr)
{
	//定义一个sockaddr_in结构来保存客户端地址信息
	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	bzero(&addr, sizeof(addr));

	// 接受套接字上的连接，将客户端信息存储在addr和addrLen中
	int clientSock = ::accept(fd, (sockaddr *)&addr, &addrLen);
	errorif(clientSock == -1, "Socket accept error");

	// 使用接受的地址和长度设置客户端地址信息
	_addr->setInetAddr(addr, addrLen);

	return clientSock;
}

void Socket::connect(InetAddress *_addr)
{
	struct sockaddr_in addr = _addr->getAddr();
	socklen_t addrLen = _addr->getAddrLen();

	// 检查文件描述符是否设置为非阻塞模式
	if (fcntl(fd, F_GETFL) & O_NONBLOCK)
	{
		// 继续尝试循环连接，直到建立成功的连接
		while (true)
		{
			// 尝试建立连接
			int ret = ::connect(fd, (sockaddr *)&addr, addrLen);
			if (ret == 0)	// 连接成功
				break;
			else if (ret == -1 and (errno == EINPROGRESS))	// 如果正在进行连接
				continue;
			else if (ret == -1)	// 连接失败
				errorif(true, "Socket connect error");
		}
	}
	else
	{
		errorif(::connect(fd, (sockaddr *)&addr, addrLen) == -1, "Socket connect error");
	}
}

int Socket::getFd()
{
	return fd;
}