#include "Socket.h"
#include "util.h"
#include "fcntl.h"

Socket::Socket() : fd(-1)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	errorif(fd == -1, "socket create error");
}

Socket::Socket(int _fd) : fd(_fd)
{
	errorif(fd == -1, "socket create error");
}

Socket::~Socket()
{
	if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
}

void Socket::bind(InetAddress const *addr)
{
	errorif(::bind(fd, (sockaddr *)&addr->addr, addr->addrLen) == -1, "socket bind error");
}

void Socket::listen()
{
	errorif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setNonBlocking()
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr)
{
	int clientfd = ::accept(fd, (sockaddr *)&addr->addr, &addr->addrLen);
	errorif(clientfd == -1, "socket accept error");
	return clientfd;
}

int Socket::getFd()
{
	return fd;
}