#include "NetWork.h"
  
int main(int argc, char **argv)  
{  
    SocketServer Server;
    Server.SocketInit("192.168.174.133",4000);
    Server.StartServer();
    return 0;
}  