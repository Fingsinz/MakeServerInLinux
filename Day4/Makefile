build:
	g++ client.cpp util.cpp -o client && \
	g++ server.cpp Epoll.cpp InetAddress.cpp Socket.cpp util.cpp -o server

clean:
	rm client && rm server