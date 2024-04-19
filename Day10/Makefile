src=$(wildcard src/*.cpp)

build:
	g++ -std=c++11 -pthread -g \
	$(src) \
	server.cpp -o server && ./\
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp \
	client.cpp -o client

clean:
	rm server && rm client

test:
	g++ src/util.cpp src/Buffer.cpp src/Socket.cpp src/InetAddress.cpp src/ThreadPool.cpp \
	-pthread \
	test.cpp -o test