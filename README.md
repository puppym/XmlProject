
### 总结
* 这次编写的基于XML的公交线路查询系统客户端用的是Qt+多线程在windows下开发,服务端用的是C++在linux下进行开发.服务端用的的服务模型是5种常见的服务模型中的第3种即多进程的模型,之前再windows下面写过的IM(即时通讯工具)是多线程,多进程的好处是单个进程崩溃会不影响整体的运行.在编写的过程中也是碰到了以下的问题,特此总结一下.

1. 在这个程序中我主要完成了客户端向服务端发送通过起始站点和结束站点查询路线的请求,和添加路线的请求.添加路线的请求只能在编辑框中输入XML形式的内容才行.例如例子1所示.服务端是先遍历XML文件组件邻接表然后通过深度优先遍历来查找最优路径.通讯部分采用socket.

2. 服务端与客户端的通讯问题：我的服务端是搭建在我虚拟机的Centos 7里面的,在与windows下的客户端通讯的过程中要关闭2者的防火墙才能进行正常的通讯.否则会收不到相应的数据包.

3. 我用的tinyxml2来解析XML文件,这个库在解析的时候如果传入的元素名错误FirstChildElement("Number"),则会直接引起程序的崩溃,这一点在使用这个库的时候要小心一点.

4. 每次添加XML数据和重新查找路线时要注意清空XmlParser类中的数据成员,因为每次查询后的结果是存在该数据成员中的.在查找路径时我是将所有的路径都查找出来了,然后存在AllRouteMap[MAXMAP]中.

5. 每个数据的结构体最好要写一个构造函数来初始化其数据.先前没写,程序崩溃过.

6. 发包组装数据的时候要注意将malloc的数据空间进行释放.

7. 在服务端碰到了一些问题Qt多线程的run()函数要写成protected;在多线程之间相互传递引用数据时要加const,const QString &name,多线程的数据安全问题;碰到的最大问题是在接收线程run()中没加QThread::run (),导致老是收到的数据包完后延后一个数据包.http://www.cnblogs.com/neopenx/p/4293599.html
说是"记得调用 QThread::run ();  否则这个run函数并没有完全执行.",现在还不是很理解;注意信号和槽的建立过程,自定义信号和槽比较像windows编程下的自定义消息.   

### 后期改进

1. 将Protocol中的结构体组成json格式再转化为字节流进行传输。




例子1：
<?xml version="1.0" encoding="UTF-8"?>
    <BusRoute>
    <Bus>
        <Number>3号线</Number>
        <Route>
            <Stop>
                <Name>东风公司</Name>
                <Distance>0</Distance>
            </Stop>
            <Stop>
                <Name>海洋乐园</Name>
                <Distance>10</Distance>
            </Stop>
            <Stop>
                <Name>园林路</Name>
                <Distance>20</Distance>
            </Stop>
            <Stop>
               <Name>江汉路</Name>
                <Distance>30</Distance>
            </Stop>
            <Stop>
                <Name>石桥路</Name>
                <Distance>40</Distance>
            </Stop>
            <Stop>
                <Name>环湖园路</Name>
                <Distance>50</Distance>
            </Stop>
        </Route>
    </Bus>
    </BusRoute>



###注意的问题
* 直接strcat竟然对于临时变量每次进入该函数时前一次的值还在,不知道什么原因,还未解决.AP_MessageData.cpp 我是每次强制回执memset

* 人民广场  1号线  武昌火车站   4号线  中南路   4号线  武汉站   最短距离为:40  这种格式,当连续的两个特殊点相邻时未解决.

* 每个客户端断开连接后,在服务端服务该客户端的进程还在运行,没有被退出.