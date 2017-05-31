#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__
#include <string>
#include <cstdio>
#include <list>
#include <vector>
#include <tinyxml2.h>
#include "VextexNode.h"

class XmlParser
{
public:
	XmlParser();
	void ReadXml(const char *FilePath);
	int FindBusRoute(std::string &start, std::string &end);
	std::list<std::string>& GetMinRoute();
	int GetMinDistant();
	int GetMinRouteIndex();
	bool AddRoute(std::string &newRoute);
	bool IsTransitNode(std::string &node);
	std::string GetRouteByTwoStopName(std::string &start, std::string &end);
	void ShowList();
private:
	//这些函数是一些辅助函数。
	void clearRouteInfo();
	void resetGp();
	int GetGpIndexByStopName(std::string &name);
	std::string GetStopNameByIndex(int num);
	int FindBusRouteByIndex(int start, int end, int distance);

private:
	tinyxml2::XMLDocument doc;
	std::list<StopNode> AllStopNode;
	GraphAdjList Gp;

	/*
	//每次查找路线完成后要将这几个变量进行清零
	bool VisitGp[MAXVEX];
	std::list<std::string> AllRouteMap[MAXMAP];
	std::list<std::string> RouteMapTmp;
	int          RouteNum;    //路线的数量
	int          MinDistant;  //最小的距离
	int          IndexRoute;  //最小距离路线的index
*/
	//记录每次的线路的结点信息，在递归的时候
	bool VisitGp[MAXVEX];
	std::list<std::string> AllRouteMap[MAXMAP];
	int          RouteMapStop[MAXVEX];
	int          RouteMapStopIndex;

	int          RouteNum;    //路线的数量
	int          MinDistant;  //最小的距离
	int          IndexRoute;  //最小距离路线的index
};

#endif