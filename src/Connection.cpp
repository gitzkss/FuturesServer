#include "Connection.h"


#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock,CuoHeServer* _chserver,Server* _server) 
: loop(_loop), sock(_sock), channel(nullptr),chServer(_chserver),server(_server){
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection(){
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            std::string recv(buf);
            solve(sockfd,recv);
            //write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb){
    deleteConnectionCallback = _cb;
}


void Connection::solve(int sockfd, std::string recv)
{
    std::map<std::string,std::string> rec = deserializeMap(recv);
    
    if(rec["op"]=="login")
        login(sockfd,rec);
    else if(rec["op"]=="weituo")
        weituo(sockfd,recv);
    else if(rec["op"]=="pankou")
    {
        pankou(sockfd);
    }
}
void Connection::login(int sockfd,std::map<std::string,std::string> rec)
{
    std::map<std::string,std::string> resp;
    if(rec["customerID"]=="1000"&&rec["password"]=="123")
    {
        resp["result"]="ok";
    }
    else
    {
        resp["type"]="login";
        resp["result"]="error";
        resp["msg"]="账号或密码错误";
    }
    std::string res = serializeMap(resp);
    sendMsg(sockfd,res);
}

void Connection::weituo(int sockfd,const std::string& recv)
{
    std::map<std::string,std::string> resp;
    WeiTuo wt = str_to_WeiTuo(recv);
    int result=chServer->verdict(wt,nullptr);
    if(result==1)
    {
        std::string msg = serializeMap(chServer->getPankou());
        server->broadCast(msg);
    }
    else if(result==0)
    {
        resp["type"]="weituo";
        resp["result"]="error";
        resp["msg"]="超出涨跌停限制";
        std::string res = serializeMap(resp);
        sendMsg(sockfd,res);
    }
}

void Connection::pankou(int sockfd)
{
    std::string res = serializeMap(chServer->getPankou());
    sendMsg(sockfd,res);
}

void Connection::sendMsg(int sockfd, const std::string& msg)
{
    write(sockfd, msg.c_str(), msg.size());
}
