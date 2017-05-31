#include "AP_MessageData.h"
#include <string>
#include <sstream>

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
    std::string start = std::string(FindRouteData.szStart);
    std::string end   = std::string(FindRouteData.szEnd);
    parser.FindBusRoute(start,end);
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
    //error   保存前一个迭代器的地址然后在解引用是程序会崩溃，mark以后自己测试一下
    std::string itLast;
    std::list<std::string>::iterator it;
    for(it = minRoute.begin(); it != minRoute.end(); it++)
    {
        /*
        strcat(FindRoute.szRouteBuf,(*it).c_str());
        strcat(FindRoute.szRouteBuf,"  ");*/
        //没有重载it+1 但是重载了++
        if(it == minRoute.begin())
        {
            /*
            bug  哈哈  直接strcat竟然对于临时变量每次进入该函数时前一次的值还在
            不知道什么原因，还未解决*/
            //std::cout << "刚开始时的数据:" << FindRoute.szRouteBuf << std::endl;
            memset(FindRoute.szRouteBuf,0,sizeof(FindRoute.szRouteBuf));
            strcat(FindRoute.szRouteBuf,(*it).c_str());
            std::cout << "添加的数据为:" << (*it) << std::endl;
            strcat(FindRoute.szRouteBuf,"  ");
            //如果第一个节点是特殊节点 就要这样写了
            itLast = *it;
            continue;
        }
        /*STL   错误的写法
        else if(ittmp++ == minRoute.end())
        {
        	strcat(FindRoute.szRouteBuf,(*it).c_str());
        	strcat(FindRoute.szRouteBuf,"  ");
        	break;
        }*/

        if(parser.IsTransitNode(*it))
        {
            std::string  end = *(it);
            std::string szRouteName = parser.GetRouteByTwoStopName(itLast,end);
            strcat(FindRoute.szRouteBuf,szRouteName.c_str());
            std::cout << "添加的数据为:" << szRouteName << std::endl;
            strcat(FindRoute.szRouteBuf,"  ");
            strcat(FindRoute.szRouteBuf,(*it).c_str());
            std::cout << "添加的数据为:" << (*it) << std::endl;
            strcat(FindRoute.szRouteBuf,"   ");
        }
        itLast = *it;
    }

    /*前面将最后一站当做特殊节点处理，这里就不许要继续添加添加最后一站
    if(it == minRoute.end())
    {
    	strcat(FindRoute.szRouteBuf,itLast.c_str());
    	strcat(FindRoute.szRouteBuf,"  ");
    }*/

    //后面优化的时候改进
    std::ostringstream streamtmp;
    streamtmp  << FindRoute.distant;
    std::string szDistance = "最短距离为:";
    szDistance += streamtmp.str();
    strcat(FindRoute.szRouteBuf,szDistance.c_str());

    std::cout << "发送的线路为:" << FindRoute.szRouteBuf << std::endl;

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
    if(NULL == pro)
    {
        return;
    }
    AddRouteRequest addRequest;
    memcpy(&addRequest,(char*)pro+sizeof(Protocol),sizeof(AddRouteRequest));
    std::cout << addRequest.szRouteName << std::endl;

    AddRouteAck addAck;
    std::string szAddRoute = std::string(addRequest.szRouteName);
    if(parser.AddRoute(szAddRoute))
    {
        addAck.flag=1;
    }
    else
    {
        addAck.flag= 0;
    }

    Protocol *sendData = (Protocol*)malloc(sizeof(Protocol)+sizeof(AddRouteAck));
    sendData->event = ADDROUTE_ACK;
    sendData->nsize = sizeof(AddRouteAck);
    memcpy((char*)sendData+sizeof(Protocol),(char*)&addAck,sizeof(AddRouteAck));
    if(send(clientSocket,(char*)sendData,sendData->nsize+sizeof(Protocol),0) < 0)
    {
        std::cout << "AddRoute_Ack send Message fail!\n" << std::endl;
        return;
    }
    std::cout <<"clientSocket: " << clientSocket <<" AddRoute_Ack send Message success!\n" << std::endl;
    free(sendData);
    return;
}

/*接上面的bug
刚开始时的数据:
金银潭  2号线  江汉路   2号线  中南路   4号线  武汉站   最短距离为:40
添加的数据为:
金银潭
XmlParser::GetRouteByTwoStopName金银潭
江汉路
添加的数据为:2号线
添加的数据为:
江汉路
XmlParser::GetRouteByTwoStopName江汉路
中南路
添加的数据为:2号线
添加的数据为:
中南路
XmlParser::GetRouteByTwoStopName圆林路
武汉站
添加的数据为:4号线
添加的数据为:
武汉站
发送的线路为:
金银潭  2号线  江汉路   2号线  中南路   4号线  武汉站   最短距离为:40金银潭  2号线  江汉路   2号线  中南路   4号线  武汉站   最短距离为:40
*/
