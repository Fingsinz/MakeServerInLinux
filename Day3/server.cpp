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
	// ���� socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	errorif(sockfd == -1, "socket create error");
	// �������Ϣ����IP���˿ڵ�
	struct sockaddr_in serverAddr;
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(1234);
	// �� socket �Լ�����
	errorif(bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1, "socket bind error");
	errorif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

	// ���� epoll
	int epfd = epoll_create1(0);
	errorif(epfd == -1, "epoll create error");
	// �����¼�����
	struct epoll_event events[MAX_EVENTS], ev;
	bzero(&events, sizeof(events));

	ev.data.fd = sockfd;			// �� IO ��Ϊ������ socket fd
	ev.events = EPOLLIN;			// �ɶ�
	setnonblocking(sockfd);			// ���� sockfd Ϊ������
	// �������� socket fd ע�ᵽ epoll
	epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

	while (true)	// ���ϼ��� epoll �ϵ��¼�������
	{
		// ��ȡ nfds �� fd �����¼�
		int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		errorif(nfds == -1, "epoll wait error");

		for (int i = 0; i < nfds; ++ i)
		{
			// ����������¼� fd �Ƿ����� socket fd����ʾ���µĿͻ�������
			if (events[i].data.fd == sockfd)
			{
				// ����ͻ��� socket ��ַ
				struct sockaddr_in clientAddr;
				bzero(&clientAddr, sizeof(clientAddr));
				socklen_t clientAddrLen = sizeof(clientAddr);
				// ���ܿͻ�������
				int clientfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
				errorif(clientfd == -1, "socket accept error");

				std::cout << "client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

				bzero(&ev, sizeof(ev));
				ev.data.fd = clientfd;			// �� IO ��Ϊ�ͻ��� socket fd
				ev.events = EPOLLIN | EPOLLET;	// �ͻ�������ʹ�� ET ģʽ
				setnonblocking(clientfd);		// ET ��Ҫ���������ʽ socket ʹ��
				// ���ͻ��� socket fd ע�ᵽ epoll
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
			}
			// ��������¼����ǿͻ��ˣ������ǿɶ��¼�
			else if (events[i].events & EPOLLIN)
			{
				char buf[READ_BUFFER];
				while (true)	// ������ IO����Ҫ���϶�ȡ��ֱ�����
				{
					bzero(buf, sizeof(buf));
					ssize_t bytesRead = read(events[i].data.fd, buf, sizeof(buf));

					// ������ȡ����
					if (bytesRead > 0)
					{
						std::cout << "Client " << events[i].data.fd << ": " << buf << std::endl;
						write(events[i].data.fd, buf, sizeof(buf));
					}
					// �ͻ��������жϣ�������ȡ
					else if (bytesRead == -1 and errno == EINTR)
					{
						std::cout << "Continue\n";
						continue;
					}
					// ������ IO�����������ʾ����ȫ����ȡ���
					else if (bytesRead == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
					{
						std::cout << "Finish reading once, errno = " << errno << "\n";
						break;
					}
					// EOF �¼���һ���ʾ�ͻ��˶Ͽ�����
					else if (bytesRead == 0)
					{
						std::cout << "EOF. Client " << events[i].data.fd << " disconnected\n";
						// �ر� socket ���Զ����ļ��������� epoll �����Ƴ�
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