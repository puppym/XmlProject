#ifndef __VEXTEXNODE_H__
#define __VEXTEXNODE_H__
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
	int          adjvex;        /* 邻接点域,存储该顶点对应的下标,下标可以通过站点名之间相互转换*/
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


#endif