#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "NetWork.h"
bool SocketServer::SocketInit(std::string ip, int port)
{
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    //创建侦听套接字
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket == 0)
    {
        printf("Create Socket Failed!");
        return false;
    }

    //一般的都是要等几分钟后才重用该端口
    //立刻重用该地址  --- 不加这一行连续运行时可能出现返回码10061
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));
    //bind
    if(bind(server_socket,(sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!",port);
        return false;
    }

    //listen
    if(listen(server_socket,LENGTH_OF_QUEUE))
    {
        printf("Server Listen Failed!");
        return false;
    }
    return true;
}

//采用多进程
bool SocketServer::StartServer()
{
    while(true)
    {
        struct sockaddr_in client_addr;
        int client_socket;

        //accept socket from clent
        socklen_t length = sizeof(client_addr);
        client_socket = accept(server_socket,(sockaddr*)&client_addr,&length);

        //创建子进程
        int ret = fork();
        switch(ret)
        {
        //error
        case -1:
        {
            printf("fork() error!");
            close(client_socket);
            close(server_socket);
            break;
        }
        //子进程
        case 0:
        {
            DealData(client_socket);
            close(client_socket);
            //这里是否需要中止进程？？？
            break;
        }
        //主进程
        default:
        {
            close(client_socket);
            continue;
        }

        }
    }
    close(server_socket);
    return true;
}


void SocketServer::DealData(int client_socket)
{
    while(1)
    {
        char buffer[BUFFER_SIZE] = "";
        bzero(buffer, BUFFER_SIZE);
        int length = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if(length < 0)
        {
            printf("Server Receieve Data Failed!\n");
            continue;
        }
        printf("%s\n",buffer);
    }
}





