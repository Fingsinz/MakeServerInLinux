#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "util.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
	// 创建 socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errorif(sockfd == -1, "socket create error");
	// 绑定相关信息，如IP、端口等
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1234);
	// 绑定 socket 以及监听
	errorif(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1, "socket bind error");
	errorif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

	// 创建 epoll
	int epfd = epoll_create1(0);
	errorif(epfd == -1, "epoll create error");
	// 定义事件数组
	struct epoll_event events[MAX_EVENTS], ev;
	bzero(&events, sizeof(events));

	ev.data.fd = sockfd;			// 该 IO 口为服务器 socket fd
	ev.events = EPOLLIN;			// 可读
	setnonblocking(sockfd);			// 设置 sockfd 为非阻塞
	// 将服务器 socket fd 注册到 epoll
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	while (true)	// 不断监听 epoll 上的事件并处理
	{
		// 获取 nfds 个 fd 发生事件
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		errorif(nfds == -1, "epoll wait error");

		for (int i = 0; i < nfds; ++ i)
		{
			// 如果发生的事件 fd 是服务器 socket fd，表示有新的客户端连接
			if (events[i].data.fd == sockfd)
			{
				// 定义客户端 socket 地址
				struct sockaddr_in clientAddr;
				bzero(&clientAddr, sizeof(clientAddr));
				socklen_t clientAddrLen = sizeof(clientAddr);
				// 接受客户端连接
				int clientfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
				errorif(clientfd == -1, "socket accept error");

				std::cout << "client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

				bzero(&ev, sizeof(ev));
				ev.data.fd = clientfd;			// 该 IO 口为客户端 socket fd
				ev.events = EPOLLIN | EPOLLET;	// 客户端连接使用 ET 模式
				setnonblocking(clientfd);		// ET 需要搭配非阻塞式 socket 使用
				// 将客户端 socket fd 注册到 epoll
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
			}
			// 如果发生事件的是客户端，并且是可读事件
			else if (events[i].events & EPOLLIN)
			{
				char buf[READ_BUFFER];
				while (true)	// 非阻塞 IO，需要不断读取，直至完毕
				{
					bzero(buf, sizeof(buf));
					ssize_t bytesRead = read(events[i].data.fd, buf, sizeof(buf));

					// 正常读取数据
					if (bytesRead > 0)
					{
						std::cout << "Client " << events[i].data.fd << ": " << buf << std::endl;
						write(events[i].data.fd, buf, sizeof(buf));
					}
					// 客户端正常中断，继续读取
					else if (bytesRead == -1 and errno == EINTR)
					{
						std::cout << "Continue\n";
						continue;
					}
					// 非阻塞 IO，这个条件表示数据全部读取完毕
					else if (bytesRead == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
					{
						std::cout << "Finish reading once, errno = " << errno << "\n";
						break;
					}
					// EOF 事件，一般表示客户端断开连接
					else if (bytesRead == 0)
					{
						std::cout << "EOF. Client " << events[i].data.fd << " disconnected\n";
						// 关闭 socket 会自动将文件描述符从 epoll 树上移除
						close(events[i].data.fd);
						break;
					}
				}
			}
			else
				std::cout << "something else happened" << std::endl;
		}
	}
	close(sockfd);
	return 0;
}