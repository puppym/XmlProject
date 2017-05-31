#ifndef __NETWORK_H__
#define __NETWORK_H__
#include "Protocol.h"
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <XmlParser.h>
#define LENGTH_OF_QUEUE 20
//这个宏和tbsys.h中的宏有重复的地方
#define BUFFER_SIZE     4096

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