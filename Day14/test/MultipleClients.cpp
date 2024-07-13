#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Buffer.h"
#include "Connection.h"
#include "ThreadPool.h"
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

static int cnt = 0;

void oneClient(int msgs, int wait)
{
	std::cout << "Enter Test\n";
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	
	Connection *conn = new Connection(nullptr, fd, cnt++);

	sleep(wait);

	int count = 0;

	while (count < msgs)
	{
		conn->setSentBuf("Client：hello");
		conn->write();
		if (conn->getState() == Connection::State::Disconnected)
		{
			conn->handleClose();
			break;
		}
		conn->read();
		std::cout << "[Msg From " << fd << " Count " << count ++ << " ]\t" << conn->getReadBuf()->c_str() << std::endl;
	}

	delete conn;
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