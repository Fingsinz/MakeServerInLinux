build:
	g++ client.cpp src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp -o client && \
	g++ server.cpp src/Acceptor.cpp src/Buffer.cpp src/Channel.cpp \
		src/Connection.cpp src/Epoll.cpp src/EventLoop.cpp \
		src/InetAddress.cpp src/Server.cpp src/Socket.cpp src/util.cpp -o server

clean:
	rm server && rm client