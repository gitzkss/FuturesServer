#pragma once

#include <map>
#include "cuo_he_server.h"
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    CuoHeServer* chserver;
    std::map<int, Connection*> connections;

public:
    Server(EventLoop*,CuoHeServer* _chserver);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *sock);
    void deleteConnection(Socket *sock);
    void broadCast(std::string msg);
};


