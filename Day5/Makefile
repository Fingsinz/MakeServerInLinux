build:
	g++ client.cpp src/util.cpp -o client && \
	g++ server.cpp src/Channel.cpp src/Epoll.cpp src/EventLoop.cpp \
		src/InetAddress.cpp src/Server.cpp src/Socket.cpp src/util.cpp \
		-o server

clean:
	rm server && rm client