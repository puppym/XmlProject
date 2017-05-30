AUTOMAKE_OPTIONS = foreign
TINYXML_ROOT = /usr/local
AM_CPPFLAGS = -std=c++11  -I/${TINYXML_ROOT}/include  -I/${TINYXML_ROOT}/include/tbsys -I/${TINYXML_ROOT}/include/tbnet

bin_PROGRAMS=Server

Server_SOURCES=Server.cpp XmlParser.cpp NetWork.cpp AP_MessageData.cpp
Server_LDADD= -lpthread $(TINYXML_ROOT)/lib/libtinyxml2.a $(TINYXML_ROOT)/lib/libtbsys.a
