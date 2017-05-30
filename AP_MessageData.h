#ifndef __AP_MESSAGEDATA_H__
#define __AP_MESSAGEDATA_H__
#include "Protocol.h"
#include "NetWork.h"
#include "XmlParser.h"
struct MessageMap
{
	int event;
	void (*pfn)(int clientSocket, XmlParser &parser, Protocol *pro);
};

#define DELCARE_MESSAGEMAP() extern MessageMap MessageMapData[];

#define BEGIN_DEFINE_MESSAGEMAP()  MessageMap MessageMapData[] = {

#define MESSAGE_ITEM(id,fun) {id,fun},

#define END_DEFINE_MESSAGEMAP() {0,NULL}};

DELCARE_MESSAGEMAP()


void ParserFindRouteRequest(int clientSocket, XmlParser &parser, Protocol *pro);
void ParserAddRouteRequest(int clientSocket, XmlParser &parser, Protocol *pro);

#endif