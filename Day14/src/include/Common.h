#pragma once

#define OS_LINUX

class Acceptor;
class Buffer;
class Channel;
class Connection;
class Epoll;
class EventLoop;
class InetAddress;
class Server;
class ThreadPool;

#define DISALLOW_COPY(className)                                                                                       \
    className(const className &) = delete;                                                                             \
    className &operator=(const className &) = delete;

#define DISALLOW_MOVE(className)                                                                                       \
    className(className &&) = delete;                                                                                  \
    className &operator=(className &&) = delete;

#define DISALLOW_COPY_AND_MOVE(className)                                                                              \
    DISALLOW_COPY(className);                                                                                          \
    DISALLOW_MOVE(className);

enum FLAG {
    FL_UNDIFINED,
    FL_SUCCESS,
    FL_SOCKET_ERROR,
    FL_EPOLL_ERROR,
    FL_CONNECTION_ERROR,
    FL_ACCEPTOR_ERROR,
    FL_UNIMPLEMENTED
};