#ifndef PROTOCOL_H
#define PROTOCOL_H
#define FINDROUTE_REQUEST   1
#define FINDROUTE_ACK       2
#define ADDROUTE_REQUEST    3
#define ADDROUTE_ACK        4

struct Protocol
{
    int event;
    int nsize;
    char pdata[];
};


struct FindRouteRequest
{
    char szStart[255];
    char szEnd[255];
};

struct FindRouteAck
{
    //如果没有查找到该公交线路 distant的值为-1
    int distant;
    char szRouteBuf[1024];
};

struct AddRouteRequest
{
    int distant;               //距离起点站的距离
    char szRouteName[1024];     //几号线
    char szStopName[255];      //站点名
};

//用整形来表示状态
struct AddRouteAck
{
    int flag;
};


#endif // PROTOCOL_H
