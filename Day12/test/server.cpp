﻿#include "../src/include/EventLoop.h"
#include "../src/include/Server.h"

int main()
{
	EventLoop *loop = new EventLoop();
	Server *server = new Server(loop);
	loop->loop();

	delete server;
	delete loop;
	return 0;
}