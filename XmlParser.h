#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__
#include <string>
#include <cstdio>
#include <list>
#include <vector>
#include <tinyxml2.h>

struct BusRouteInfo               //节点公共汽车存在哪些路线上
{
	std::string RouteName;
	std::string StartStop;
	std::string EndStop;
	int         Distant;  
};

struct StopNode
{
	bool flag;                    //是否为特殊节点
	std::vector<BusRouteInfo> v;  //节点公共汽车存在哪些路线上
	std::string            StopName;
	int                    GpNum; //在邻接表中的位置
	StopNode()
	{
		flag = false;
		GpNum = 0;
	}
};

#define MAXMAP   10            /*最大能够查询的路线为10条*/
#define MAXVEX   100           /* 最大顶点数,应由用户定义 */
typedef StopNode VertexType;   /* 顶点类型应由用户定义 */
typedef int      EdgeType;     /* 边上的权值类型应由用户定义 */

typedef struct EdgeNode        /* 边表结点  */
{
	int  adjvex;        /* 邻接点域,存储该顶点对应的下标,下标可以通过站点名之间相互转换*/
	EdgeType     weight;       /* 用于存储权值,对于非网图可以不需要 */
	struct EdgeNode *next;     /* 链域,指向下一个邻接点 */
	EdgeNode()
	{
		adjvex = 0;
		weight = 0;
		next = NULL;
	}
}EdgeNode;

typedef struct VextexNode      /* 顶点表结点 */
{
	VertexType data;           /* 顶点域,存储顶点信息 */
	EdgeNode   *firstedge;     /* 边表头指针 */
	VextexNode()
	{
		firstedge = NULL;
	}
}VextexNode, AdjList[MAXVEX];

typedef struct 
{
	AdjList adjList;
	int     numNodes;          /* 图中当前顶点数和边数 */
	int     numEdges;
}GraphAdjList;


class XmlParser
{
public:
	XmlParser();
	void ReadXml(const char *FilePath);
	int GetGpIndexByStopName(std::string name);
	std::string GetStopNameByIndex(int num);
	int FindBusRoute(std::string start, std:: string end);
	int FindBusRouteByIndex(int start, int end, int distance);
	void ShowList();
private:
	void clearRouteInfo();
	tinyxml2::XMLDocument doc;
	std::list<StopNode> AllStopNode;
	GraphAdjList Gp;

	//每次查找路线完成后要将这几个变量进行清零
	bool VisitGp[MAXVEX];
	std::list<std::string> AllRouteMap[MAXMAP];
	//记录每次的线路的结点信息，在递归的时候
	int          RouteMapStop[MAXVEX];
	int          RouteMapStopIndex;

	int          RouteNum;    //路线的数量
	int          MinDistant;  //最小的距离
	int          IndexRoute;  //最小距离路线的index
};

#endif