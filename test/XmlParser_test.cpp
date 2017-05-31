#include "XmlParser.h"
#include <list>
#include <tbsys.h>

int main()
{
    //论"园林路" 与 "圆林路"  的区别  调了快2个小时  。。。。最近一直在翻这种低级错误。"知耻而后勇"
	std::string newRoute = 
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
	"<BusRoute>"
    "<Bus>"
        "<Number>3号线</Number>"
        "<Route>"
            "<Stop>"
                "<Name>东风公司</Name>"
                "<Distance>0</Distance>"
            "</Stop>"
            "<Stop>"
                "<Name>海洋乐园</Name>"
                "<Distance>10</Distance>"
            "</Stop>"
            "<Stop>"
                "<Name>圆林路</Name>"
                "<Distance>20</Distance>"
            "</Stop>"
            "<Stop>"
               "<Name>江汉路</Name>"
                "<Distance>30</Distance>"
            "</Stop>"
            "<Stop>"
                "<Name>石桥路</Name>"
                "<Distance>40</Distance>"
            "</Stop>"
            "<Stop>"
                "<Name>环湖园路</Name>"
                "<Distance>50</Distance>"
            "</Stop>"
        "</Route>"
    "</Bus>"
    "</BusRoute>";
	XmlParser tmp;
	tmp.ReadXml("BusRoute.xml");
    std::string start = "金银潭";
    std::string end = "武汉站";
	tmp.FindBusRoute(start,end);
	TBSYS_LOG(DEBUG,"start show!\n");
	tmp.ShowList();



    std::cout <<"重新查找路线";
    tmp.AddRoute(newRoute);
    tmp.FindBusRoute(start,end);
    tmp.ShowList();
}