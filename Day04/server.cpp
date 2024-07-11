#include <iostream>
#include <cstring>
#include <utility>
#include "util.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"

constexpr int maxEvents = 1024;
constexpr int bufferSize = 1024;

/**
 * @brief ����ͻ�������
 *
 * @param clientfd �ͻ������ӵ��ļ�������
 */
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
		// �ͻ��������жϡ�������ȡ
		else if (len == -1 and errno == EINTR)
		{
			std::cout << "Continue\n";
			continue;
		}
		// ������ IO����ʾ����ȫ����ȡ���
		else if (len == -1 and ((errno == EAGAIN) or (errno == EWOULDBLOCK)))
		{
			// �������£�������ΪEAGAIN��EWOULDBLOCK
			std::cout << "Finished reading once\n";
			break;
		}
		// EOF����ʾ�ͻ��˶Ͽ�����
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
	// ���������� Socket
	Socket *serverSock = new Socket();
	InetAddress *serverAddr = new InetAddress("127.0.0.1", 1234);
	serverSock->bind(serverAddr);
	serverSock->listen();
	serverSock->setNonBlocking();
	// ���� Epoll ����
	Epoll *epoll = new Epoll();
	epoll->addFd(serverSock->getFd(), EPOLLIN);
	// ����ͻ����ڴ�
	vector<std::pair<InetAddress *, Socket *>> clients;

	while (true)
	{
		// ��ѯ�¼�
		vector<epoll_event> events = epoll->poll();

		for (epoll_event const &e : events)
		{
			// �¼��� fd Ϊ������ fd����ʾ���¿ͻ�������
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
			// �¼��� fd Ϊ�ɶ�����ʾ�пͻ��˷���������
			else if (e.events & EPOLLIN)
			{
				handleClient(e.data.fd);
			}
			else // ����
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