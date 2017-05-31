#include "XmlParser.h"
#include <tbsys.h>
#include <sstream>
using namespace tinyxml2;


XmlParser::XmlParser()
{
    Gp.numNodes = 0;
    Gp.numEdges = 0;
    RouteNum = 0;
    MinDistant = 10000000;
    IndexRoute = 0;
    memset(VisitGp,0,MAXVEX);
    for(int i = 0; i < MAXVEX; i++)
    {
        RouteMapStop[i] = -1;
    }
    RouteMapStopIndex = 0;
}

int XmlParser::GetGpIndexByStopName(std::string &name)
{
    for(int i = 0; i < Gp.numNodes; i++)
    {
        StopNode tmp = Gp.adjList[i].data;
        if(tmp.StopName == name)
        {
            return tmp.GpNum;
        }
    }
    return -1;
}

std::string XmlParser::GetStopNameByIndex(int num)
{
    for(int i = 0; i < Gp.numNodes; i++)
    {
        StopNode tmp = Gp.adjList[i].data;
        if(tmp.GpNum == num)
        {
            return tmp.StopName;
        }
    }
    return "";
}

std::list<std::string>& XmlParser::GetMinRoute()
{
    return AllRouteMap[IndexRoute-1];
}

int XmlParser::GetMinDistant()
{
    return MinDistant;
}

int XmlParser::GetMinRouteIndex()
{
    return IndexRoute;
}

void XmlParser::resetGp()
{
    for(int i = 0; i < Gp.numNodes; i++)
    {
        Gp.adjList[i].data.flag = false;
        Gp.adjList[i].data.v.clear();
        Gp.adjList[i].data.GpNum = 0;
        EdgeNode *pTemp =  Gp.adjList[i].firstedge;
        //error  这里刚开时的时候没有进行循环遍历
        while(pTemp)
        {
            Gp.adjList[i].firstedge = pTemp->next;
            free(pTemp);
            pTemp = Gp.adjList[i].firstedge;
        }
    }

    Gp.numNodes = 0;
    Gp.numEdges = 0;
}

bool XmlParser::IsTransitNode(std::string &node)
{
    //为了满足此处建立信息包的要求将结束站也设为特殊节点
    if(0 != GetMinRouteIndex())
    {
        std::list<std::string> minRoute = GetMinRoute();
        if(0 == strcmp(minRoute.back().c_str(),node.c_str()))
        {
            return true;
        }
    }

    //真正的每条线路的结点
    for(std::list<StopNode>::iterator it = AllStopNode.begin(); it != AllStopNode.end(); it++)
    {
        if(0 == strcmp((*it).StopName.c_str(),node.c_str()))
        {
            return (*it).flag;
        }
    }
    return false;
}

std::string XmlParser::GetRouteByTwoStopName(std::string &start, std::string &end)
{
    std::cout << "XmlParser::GetRouteByTwoStopName";
    std::cout << start << std::endl;
    std::cout << end << std::endl;
    for(std::list<StopNode>::iterator it = AllStopNode.begin(); it != AllStopNode.end(); it++)
    {
        if(0 == strcmp((*it).StopName.c_str(),start.c_str()))
        {
            for(std::list<StopNode>::iterator _it = AllStopNode.begin(); _it != AllStopNode.end(); _it++)
            {
                if(0 == strcmp((*_it).StopName.c_str(),end.c_str()))
                {
                    for(std::vector<BusRouteInfo>::iterator it_start = (*it).v.begin(); it_start != (*it).v.end(); it_start++)
                    {
                        for(std::vector<BusRouteInfo>::iterator it_end = (*_it).v.begin(); it_end != (*_it).v.end(); it_end++)
                        {
                            if(0 == strcmp((*it_start).RouteName.c_str(),(*it_end).RouteName.c_str()))
                            {
                                return (*it_end).RouteName;
                            }
                        }
                    }
                }
            }
        }
    }
    return "";
}

void XmlParser::ReadXml(const char *FilePath)
{
    TBSYS_LOGGER.setFileName("XMLParser.log.txt",true);
    TBSYS_LOGGER.setLogLevel("debug");

    //重置内存中的公交线路的所有变量
    doc.Clear();
    AllStopNode.clear();
    //memset(&Gp,0,sizeof(Gp));
    resetGp();
    doc.LoadFile(FilePath);
    XMLElement *root = doc.RootElement();
    XMLElement *ElementBus = root->FirstChildElement("Bus");
    TBSYS_LOG(DEBUG,"get root and start parser!");
    int nCount = 0;
    while(ElementBus)
    {
        //路线名
        std::string RouteName =  ElementBus->FirstChildElement("Number")->GetText();
        XMLElement *ElementRoute    = ElementBus->FirstChildElement("Route");
        XMLElement *ElementStartStop = ElementRoute->FirstChildElement("Stop");
        //该条路线的开始站点
        std::string StartStop        = ElementStartStop->FirstChildElement()->GetText();
        XMLElement *ElementEndStop   = ElementRoute->LastChildElement("Stop");
        //该条路线的结束站点
        std::string EndStop          = ElementEndStop->FirstChildElement()->GetText();
        XMLElement *pTemp = ElementStartStop;
        StopNode         StoptmpLast;  //存储上一个节点的值，用于构造邻接表
        TBSYS_LOG(DEBUG,"get root and start parser and do while! %s %s %s",RouteName.c_str(),StartStop.c_str(),EndStop.c_str());
        while(pTemp)
        {
            StopNode     Stoptmp;
            BusRouteInfo Bustmp;
            TBSYS_LOG(DEBUG,"entry!");
            std::string  CurrentStopName = pTemp->FirstChildElement("Name")->GetText();
            std::string  CurrentStopDistant = pTemp->FirstChildElement("Distance")->GetText();
            TBSYS_LOG(DEBUG,"get root and start parser and do deal! %s %s",CurrentStopDistant.c_str(),CurrentStopName.c_str());
            int Distant = 0;
            //std::istringstream 将字符串转化为任何类型
            //std::ostringstream 将任何类型转化为字符串
            std::istringstream streamtmp;
            streamtmp.str(CurrentStopDistant);
            streamtmp>>Distant;

            Bustmp.RouteName = RouteName;
            Bustmp.StartStop = StartStop;
            Bustmp.EndStop   = EndStop;
            Bustmp.Distant   = Distant;
            Stoptmp.StopName = CurrentStopName;
            Stoptmp.v.push_back(Bustmp);
            Stoptmp.flag = false;
            Stoptmp.GpNum = 0;

            TBSYS_LOG(DEBUG,"get root and start parser and do deal! %d %s",Distant,CurrentStopName.c_str());
            std::list<StopNode>::iterator plist;


            //分辨特殊节点
            for(plist = AllStopNode.begin(); plist != AllStopNode.end(); plist++)
            {
                if(Stoptmp.StopName != (*plist).StopName)
                {
                    //error logical
                    continue;
                }
                else
                {
                    TBSYS_LOG(DEBUG,"当前节点是特殊节点 %s",Stoptmp.StopName.c_str());
                    //只添加公汽车的路线信息到里面
                    (*plist).v.push_back(Bustmp);
                    (*plist).flag = true;        //是特殊节点
                    //error 对于特殊节点的处理这里有问题 //error
                    Stoptmp.GpNum = GetGpIndexByStopName(Stoptmp.StopName);
                    break;
                }
            }
            //相当于是输入顶点信息
            if(plist == AllStopNode.end())
            {
                Stoptmp.GpNum = nCount;
                AllStopNode.push_back(Stoptmp);
                //将顶点按照遍历的先后顺序依次放入邻接表的表头节点
                Gp.adjList[nCount].data = Stoptmp;
                Gp.adjList[nCount].firstedge = NULL;
                Gp.numNodes++;
                nCount++;
            }

            //构造邻接表
            //可能是特殊节点
            //也可能不是特殊节点
            //当前节点往前进行构造
            if(Stoptmp.StopName != Bustmp.StartStop)
            {
                TBSYS_LOG(DEBUG,"当前节点:%s",Stoptmp.StopName.c_str());
                TBSYS_LOG(DEBUG,"当前节点 index:%d",Stoptmp.GpNum);
                TBSYS_LOG(DEBUG,"上一个节点:%s",StoptmpLast.StopName.c_str());
                TBSYS_LOG(DEBUG,"当前节点 index:%d",StoptmpLast.GpNum);
                Gp.numEdges++;
                //当前节点插入到上一个顶点的后面
                EdgeNode *pe = (EdgeNode*)malloc(sizeof(EdgeNode));
                //mark StoptmpLast.GpNum; 逻辑错误
                pe->adjvex   = Stoptmp.GpNum;
                pe->weight   = (Stoptmp.v.front()).Distant - (StoptmpLast.v.front()).Distant;
                int num = GetGpIndexByStopName(StoptmpLast.StopName);
                if(-1 == num)
                {
                    //mark 注意构造函数的数值初始化
                    printf("该节点还未存入邻接表的表头顶点,当前边构造失败");
                    continue;
                }

                pe->next = Gp.adjList[num].firstedge;
                Gp.adjList[num].firstedge = pe;

                //前一个顶点插入到当前节点的后面
                pe = (EdgeNode*)malloc(sizeof(EdgeNode));
                pe->adjvex = StoptmpLast.GpNum;
                pe->weight   = (Stoptmp.v.front()).Distant - (StoptmpLast.v.front()).Distant;
                num = GetGpIndexByStopName(Stoptmp.StopName);
                if(-1 == num)
                {
                    printf("该节点还未存入邻接表的表头顶点,当前边构造失败");
                    continue;
                }
                pe->next = Gp.adjList[num].firstedge;
                Gp.adjList[num].firstedge = pe;
            }


            StoptmpLast = Stoptmp;
            pTemp = pTemp->NextSiblingElement();
        }
        ElementBus = ElementBus->NextSiblingElement();
    }
}

/*每次查找路线完成后要将这几个变量进行清零
    bool VisitGp[MAXVEX];
    std::list<std::string> AllRouteMap[MAXMAP];
    int          RouteMapStop[MAXVEX];
    int          RouteMapStopIndex;

    int          RouteNum;    //路线的数量
    int          MinDistant;  //最小的距离
    int          IndexRoute;  //最小距离路线的index
*/
void XmlParser::clearRouteInfo()
{
    memset(VisitGp,0,MAXVEX);
    for(int i = 0; i < MAXMAP; i++)
    {
        AllRouteMap[i].clear();
    }
    RouteNum = 0;
    MinDistant = 10000000;
    IndexRoute = 0;
    for(int i = 0; i < MAXVEX; i++)
    {
        RouteMapStop[i] = -1;
    }
    RouteMapStopIndex = 0;
}

int XmlParser::FindBusRoute(std::string &start, std:: string &end)
{
    clearRouteInfo();
    int startIndex = GetGpIndexByStopName(start);
    int endIndex   = GetGpIndexByStopName(end);
    if(-1 == startIndex || -1 == endIndex)
    {
        std::cout << "该起始站点不在计划的公交线路类!" << std::endl;
        //没有查找到该公交站点
        MinDistant = -1;
        return 0;

    }
    return FindBusRouteByIndex(startIndex,endIndex,0);
}

/*
    std::list<std::string> AllRouteMap[MAXMAP];
	std::list<std::string> RouteMapTmp;
    int          RouteNum;    //路线的数量
	int          MinDistant;  //最小的距离
	int          IndexRoute;  //最小距离路线的index
	*/
int XmlParser::FindBusRouteByIndex(int start, int end, int distant)
{
    if(start == end)
    {
        //起始站点和终点站点一样的话有客户端进行校验
        //添加到总共的路线中去
        RouteMapStop[RouteMapStopIndex] = end;
        //RouteMapTmp.push_back(Gp.adjList[end].data.StopName);

        std::list<std::string> RouteMapTmp;
        RouteMapStopIndex++;
        for(int i = 0; i < RouteMapStopIndex; i++)
        {
            RouteMapTmp.push_back(GetStopNameByIndex(RouteMapStop[i]));
        }

        AllRouteMap[RouteNum] = RouteMapTmp;
        RouteMapTmp.clear();
        RouteNum++;                           //查找线路的数量
        if(MinDistant > distant)
        {
            MinDistant = distant;
            IndexRoute = RouteNum;
        }
        return 0;
    }
    //erroe STL 分配在堆中  递归时不会对堆中的数据进行备份
    //RouteMapTmp.push_back(Gp.adjList[start].data.StopName);
    RouteMapStop[RouteMapStopIndex] = start;
    RouteMapStopIndex++;

    //mark
    VisitGp[start] = true;
    EdgeNode *pTemp = Gp.adjList[start].firstedge;
    while(pTemp)
    {
        if(!VisitGp[pTemp->adjvex])
        {
            VisitGp[pTemp->adjvex] = true;
            FindBusRouteByIndex(pTemp->adjvex,end,distant+pTemp->weight);
            //error忘记回执该记录节点
            VisitGp[pTemp->adjvex] = false;
            RouteMapStopIndex--;
        }
        pTemp = pTemp->next;
    }

    return 0;

}

bool XmlParser::AddRoute(std::string &newRoute)
{
    std::cout << "开始插入元素" << std::endl;
    //std::cout << newRoute << std::endl;
    tinyxml2::XMLDocument parser;
    parser.Parse(newRoute.c_str());
    XMLElement *root = doc.RootElement();
    XMLElement *newRoot = parser.RootElement();
    XMLElement *addBusRoute = newRoot->FirstChildElement("Bus")->FirstChildElement("Route")->FirstChildElement("Stop");

    //mark 如果将元素名称写错了这里调用API时会崩溃
    //创建新的3个元素
    XMLElement *pBus = doc.NewElement("Bus");
    XMLElement *pNewRoute = doc.NewElement("Route");
    XMLElement *pRouteName = doc.NewElement("Number");

    //插入Name
    std::string szRouteName = newRoot->FirstChildElement("Bus")->FirstChildElement("Number")->GetText();
    pRouteName->InsertFirstChild(doc.NewText(szRouteName.c_str()));
    pBus->InsertFirstChild(pRouteName);
    //插入Route
    while(addBusRoute)
    {
        XMLElement *tmpStop = doc.NewElement("Stop");
        XMLElement *tmpDistance = doc.NewElement("Distance");
        XMLElement *tmpName = doc.NewElement("Name");

        //创建Stop
        std::string distance = addBusRoute->FirstChildElement("Distance")->GetText();
        std::string name    = addBusRoute->FirstChildElement("Name")->GetText();
        tmpDistance->InsertEndChild(doc.NewText(distance.c_str()));
        tmpName->InsertEndChild(doc.NewText(name.c_str()));
        tmpStop->InsertEndChild(tmpName);
        tmpStop->InsertEndChild(tmpDistance);

        //连接每个Stop
        pNewRoute->InsertEndChild(tmpStop);

        addBusRoute = addBusRoute->NextSiblingElement();

    }

    pBus->InsertEndChild(pNewRoute);
    //只能在相同的XML文件内这样整体的移动不能在不同的XML文件内整体的移动
    root->InsertFirstChild(pBus);

    /*将XML文件转化为字符串
    XMLPrinter printer1(0,true);
    doc.Accept(&printer1);
    std::cout << printer1.CStr() << std::endl;*/


    if(0 == doc.SaveFile("BusRoute.xml"))
    {
        std::cout << "SaveFile success!" << std::endl;
        ReadXml("BusRoute.xml");
        return true;
    }
    return false;
}

void XmlParser::ShowList()
{
    TBSYS_LOG(DEBUG,"entry ShowList");
    for(std::list<StopNode>::iterator plist = AllStopNode.begin(); plist != AllStopNode.end(); plist++)
    {
        if(true == (*plist).flag)
        {
            std::cout << (*plist).StopName << " " ;
        }
    }

    std::cout << AllStopNode.size() << " ";
    std::cout << "Gp.numNodes = " << Gp.numNodes  << " ";
    std::cout << "Gp.numEdges = " << Gp.numEdges  << " ";
    for(int i = 0; i < Gp.numNodes; i++)
    {
        std::cout << Gp.adjList[i].data.StopName << " " << Gp.adjList[i].data.GpNum << " " ;
    }

    for(int i = 0; i < Gp.numNodes; i++)
    {
        std::cout << Gp.adjList[i].data.StopName << " " << std::endl;
        EdgeNode *pTemp = Gp.adjList[i].firstedge;
        while(pTemp)
        {
            std::cout <<"index:" << pTemp->adjvex << " " << "stop:" << GetStopNameByIndex(pTemp->adjvex) << " " << "Distant:" << pTemp->weight << std::endl;
            pTemp = pTemp->next;
        }
        std::cout << std::endl;
    }

    std::cout << "MinDistant = " << MinDistant << std::endl;
    std::cout << "RouteNum = " <<   RouteNum << std::endl;
    std::cout << "IndexRoute = " << IndexRoute << std::endl;
    for(int i = 0; i < RouteNum; i++)
    {
        for(std::list<std::string>::iterator it = AllRouteMap[i].begin(); it != AllRouteMap[i].end(); it++)
        {
            std::cout << (*it) << " ";
        }
        std::cout << "*****" <<std::endl;
    }

}
/*
	//每次查找路线完成后要将这几个变量进行清零
	bool VisitGp[MAXVEX];
	std::list<std::string> AllRouteMap[MAXMAP];
	std::list<std::string> RouteMapTmp;
	int          RouteNum;    //路线的数量
	int          MinDistant;  //最小的距离
	int          IndexRoute;  //最小距离路线的index
*/
