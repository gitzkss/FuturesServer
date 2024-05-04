#include "src/EventLoop.h"
#include "src/Server.h"
#include "src/cuo_he_server.h"
int main() {
    EventLoop *loop = new EventLoop();
    CuoHeServer* chserver = new CuoHeServer(1500,1600,1400);
    Server *server = new Server(loop,chserver);
    loop->loop();
    delete server;
    delete chserver;
    delete loop;
    return 0;
}
