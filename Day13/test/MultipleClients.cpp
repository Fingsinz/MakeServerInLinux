﻿#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Connection.h"
#include "InetAddress.h"
#include "Socket.h"
#include "ThreadPool.h"

using namespace std;

void oneClient(int msgs, int wait)
{
	Socket *sock = new Socket();
	InetAddress *addr = new InetAddress("127.0.0.1", 1234);
	sock->connect(addr);
	Connection *conn = new Connection(nullptr, sock);

	sleep(wait);

	int count = 0;

	while (count < msgs)
	{
		conn->setSentBuffer("Client：hello");
		conn->write();
		if (conn->getState() == Connection::State::Closed)
		{
			conn->close();
			break;
		}
		conn->read();
		std::cout << "[Msg From " << sock->getFd() << " Count " << count ++ << " ]\t" << conn->readBuffer() << std::endl;
	}

	delete conn;
	delete addr;
	// delete sock; Connection 类会释放 Socket
}

int main(int argc, char *argv[])
{
	int threads = 100;
	int msgs = 100;
	int wait = 0;
	int o = -1;
	const char *optstring = "t:m:w:";
	while ((o = getopt(argc, argv, optstring)) != -1)
	{
		switch (o)
		{
			case 't':
				threads = stoi(optarg);
				break;
			case 'm':
				msgs = stoi(optarg);
				break;
			case 'w':
				wait = stoi(optarg);
				break;
			case '?':
				printf("error optopt: %c\n", optopt);
				printf("error opterr: %d\n", opterr);
				break;
		}
	}

	ThreadPool *poll = new ThreadPool(threads);
	std::function<void()> func = std::bind(oneClient, msgs, wait);
	for (int i = 0; i < threads; ++i)
	{
		poll->add(func);
	}
	delete poll;
	return 0;
}