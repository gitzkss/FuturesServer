#pragma once
#include <functional>
#include <cstdio>
#include <cerrno>
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include <map>
#include <string>
#include "cuo_he_server.h"
#include "Server.h"

class EventLoop;
class Socket;
class Channel;
class Connection
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *channel;
    CuoHeServer* chServer;
    Server* server;
    std::function<void(Socket*)> deleteConnectionCallback;
public:
    Connection(EventLoop *_loop, Socket *_sock,CuoHeServer* _chserver,Server* _server);
    ~Connection();
    
    void solve(int sockfd, std::string recv);
    void login(int sockfd,std::map<std::string,std::string> rec);
    void weituo(int sockfd,const std::string& recv);
    void pankou(int sockfd);
    void sendMsg(int sockfd, const std::string& msg);
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};

