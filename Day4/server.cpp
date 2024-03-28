#include <iostream>
#include <cstring>
#include <utility>
#include "util.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"

constexpr int maxEvents = 1024;
constexpr int bufferSize = 1024;

void handleClient(int clientfd)
{
	char buffer[bufferSize];
	while (true)
	{
		bzero(buffer, sizeof(buffer));
		ssize_t len = read(clientfd, buffer, sizeof(buffer));

		if (len > 0)
		{
			std::cout << "From client " << clientfd << ": " << buffer << "\n";
			write(clientfd, buffer, sizeof(buffer));
		}
		// 客户端正常中断、继续读取
		else if (len == -1 and errno == EINTR)
		{
			std::cout << "Continue\n";
			continue;
		}
		// 非阻塞 IO，表示数据全部读取完毕
		else if (len == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			// 非阻塞下，错误码为EAGAIN或EWOULDBLOCK
			std::cout << "Finished reading once\n";
			break;
		}
		// EOF，表示客户端断开连接
		else if (len == 0)
		{
			std::cout << "Client " << clientfd << " disconnected\n";
			close(clientfd);
			break;
		}
	}
}

int main()
{
	Socket *serverSock = new Socket();
	InetAddress *serverAddr = new InetAddress("127.0.0.1", 1234);
	serverSock->bind(serverAddr);
	serverSock->listen();
	serverSock->setNonBlocking();

	Epoll *epoll = new Epoll();
	epoll->addFd(serverSock->getFd(), EPOLLIN);

	vector<std::pair<InetAddress *, Socket *>> clients;

	while (true)
	{
		vector<epoll_event> events = epoll->poll();

		for (epoll_event const &e : events)
		{
			if (e.data.fd == serverSock->getFd())
			{
				InetAddress *clientAddr = new InetAddress();
				Socket *clientSock = new Socket(serverSock->accept(clientAddr));
				clients.push_back({ clientAddr, clientSock });

				std::cout << "New client " << clientSock->getFd() << ": " <<
					inet_ntoa(clientAddr->addr.sin_addr) << " : " << ntohs(clientAddr->addr.sin_port) << "\n";
				clientSock->setNonBlocking();
				epoll->addFd(clientSock->getFd(), EPOLLIN | EPOLLET);
			}
			else if (e.events & EPOLLIN)
			{
				handleClient(e.data.fd);
			}
			else
				std::cout << "Something else happened\n";
		}
	}

	for (std::pair<InetAddress *, Socket *> client : clients)
		delete client.first, delete client.second;

	delete serverSock;
	delete serverAddr;
	delete epoll;
	return 0;
}