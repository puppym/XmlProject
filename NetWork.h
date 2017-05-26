#ifndef __NETWORK_H__
#define __NETWORK_H__
#include <string>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#define LENGTH_OF_QUEUE 20
#define BUFFER_SIZE     1024

class SocketServer
{
public:
	bool SocketInit(std::string ip, int port);
	bool StartServer();
	void DealData(int client_socket);
private:
	struct sockaddr_in server_addr;
	int server_socket;
	int opt;
};


#endif