AUTOMAKE_OPTIONS = foreign
TINYXML_ROOT = /usr/local
AM_CPPFLAGS = -std=c++11  -I/${TINYXML_ROOT}/include  -I/${TINYXML_ROOT}/include/tbsys -I/${TINYXML_ROOT}/include/tbnet

bin_PROGRAMS=XmlParser_test

XmlParser_test_SOURCES=XmlParser_test.cpp ../XmlParser.cpp
XmlParser_test_LDADD=-lpthread $(TINYXML_ROOT)/lib/libtinyxml2.a $(TINYXML_ROOT)/lib/libtbsys.a