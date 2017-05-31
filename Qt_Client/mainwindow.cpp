#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtNetWork>
#include <QTcpSocket>
#include "clientthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_threadSocket = new ClientThread();
    connect(m_threadSocket,SIGNAL(changeRouteText(const QString &)),this,SLOT(changeRouteEdit(const QString &)),Qt::QueuedConnection);
    connect(this, SIGNAL(sendFindData(const QString&,const QString&)),m_threadSocket,SLOT(sendData(const QString&,const QString&)),Qt::QueuedConnection);
    //void sendAddData(const QString&);
    connect(this, SIGNAL(sendAddData(const QString&)),m_threadSocket,SLOT(receiveAddData(const QString&)),Qt::QueuedConnection);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(GetEditText()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(SendMainEditText()));
    m_threadSocket->start();
    setWindowTitle(tr("公交路线查询系统"));
    //m_threadSocket->start();
    /*
    char str[1024] = "hello qt!";
    QTcpSocket *tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress("192.168.174.133"),4000);
    tcpSocket->write(str);
    QMessageBox::warning(this,tr("spreadsheet"),
                   tr(str),
                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                    */
}

void MainWindow::GetEditText()
{
    QString start = ui->lineEdit_start->text();
    QString end   = ui->lineEdit_end->text();
    qDebug() << "start = " << start << endl;
    qDebug() << "end = " << end << endl;
    emit sendFindData(start,end);
}

void MainWindow::SendMainEditText()
{
    QString addRoute = ui->textEdit_route->toPlainText();
    emit sendAddData(addRoute);
}

void MainWindow::changeRouteEdit(const QString &Route)
{
    //这里没清除
    ui->textEdit_route->clear();
    ui->textEdit_route->setText(Route);
}

MainWindow::~MainWindow()
{
    delete ui;
}
