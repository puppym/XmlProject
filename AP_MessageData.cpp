#include "AP_MessageData.h"
#include <string>

BEGIN_DEFINE_MESSAGEMAP()
MESSAGE_ITEM(FINDROUTE_REQUEST,ParserFindRouteRequest)
MESSAGE_ITEM(ADDROUTE_REQUEST,ParserAddRouteRequest)
END_DEFINE_MESSAGEMAP()

void ParserFindRouteRequest(int clientSocket, XmlParser &parser, Protocol *pro)
{
	if(NULL == pro)
	{
		return;
	}
	FindRouteRequest FindRouteData;
	memcpy(&FindRouteData,pro->pdata,sizeof(FindRouteRequest));
	std::cout << "起始站点：" << FindRouteData.szStart << std::endl;
	std::cout << "结束站点：" << FindRouteData.szEnd << std::endl;
	parser.FindBusRoute(std::string(FindRouteData.szStart),std::string(FindRouteData.szEnd));
	FindRouteAck  FindRoute;
	//没有查找到路线distant是返回-1
	FindRoute.distant = parser.GetMinDistant();
	std::list<std::string> minRoute;
	//这里也可以用FindRoute.IndexRoute最优线路标号进行校验
	//标号为0则未查找到
	if(0 != parser.GetMinRouteIndex())
	{
		minRoute = parser.GetMinRoute();
	}
	/*
	for(int i = 0; i < minRoute.size(); i++)
	{
		strcat(FindRoute.szRouteBuf,(minRoute.).c_str());
		strcat(FindRoute.szRouteBuf,"  ");
	}*/
	for(std::list<std::string>::iterator it = minRoute.begin(); it != minRoute.end(); it++)
	{
		strcat(FindRoute.szRouteBuf,(*it).c_str());
		strcat(FindRoute.szRouteBuf,"  ");
	}

	//收包不需要malloc  只是发包要malloc一段地址
	Protocol *sendData = (Protocol*)malloc(sizeof(Protocol)+sizeof(FindRouteAck));
	sendData->event = FINDROUTE_ACK;
	sendData->nsize = sizeof(FindRouteAck);
	memcpy((char*)sendData+sizeof(Protocol),(char*)&FindRoute,sizeof(FindRouteAck));
	if(send(clientSocket,(char*)sendData,sendData->nsize+sizeof(Protocol),0) < 0)
	{
		std::cout << "FindRoute_Ack send Message fail!\n" << std::endl;
		return;
	}
	std::cout <<"clientSocket: " << clientSocket <<"FindRoute_Ack send Message success!\n" << std::endl;
	free(sendData);
	return;
}

void ParserAddRouteRequest(int clientSocket, XmlParser &parser, Protocol *pro)
{
	return;
}
