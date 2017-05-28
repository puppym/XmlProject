#include "XmlParser.h"
#include <list>
#include <tbsys.h>

int main()
{
	XmlParser tmp;
	tmp.ReadXml("../BusRoute.xml");
	tmp.FindBusRoute("街道口","人民广场");
	TBSYS_LOG(DEBUG,"start show!\n");
	tmp.ShowList();
}