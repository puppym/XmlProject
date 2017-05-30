#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//情况2
    //这里要转换为UTF-8的格式
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    //mark  QString 使用UTF-8的格式
    QTextCodec::setCodecForCStrings(codec);
    MainWindow w;
    w.show();

    return a.exec();
}
