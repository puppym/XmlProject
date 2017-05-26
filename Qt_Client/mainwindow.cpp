#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtNetWork>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    char str[1024] = "hello qt!";
    QTcpSocket *tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress("192.168.174.133"),4000);
    tcpSocket->write(str);
    QMessageBox::warning(this,tr("spreadsheet"),
                   tr(str),
                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
