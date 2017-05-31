#include "XmlParser.h"
#include "Protocol.h"
#include "AP_MessageData.h"
#include <list>

int main()
{
	XmlParser tmp;
	tmp.ReadXml("BusRoute.xml");

	std::string start = "街道口";
	std::string end   = "人民广场";
	Protocol *pStr = (Protocol*)malloc(sizeof(Protocol)+ sizeof(FindRouteRequest));
    FindRouteRequest RouteData;
    pStr->event = FINDROUTE_REQUEST;
    pStr->nsize = sizeof(FindRouteRequest);
    strcpy(RouteData.szStart,start.c_str());
    strcpy(RouteData.szEnd,end.c_str());
    memcpy((char*)pStr+sizeof(Protocol),(char*)&RouteData,sizeof(FindRouteRequest));

    ParserFindRouteRequest(0,tmp,pStr);
	return 0;
}