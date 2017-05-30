#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
   void changeRouteEdit(const QString &Route);
   void GetEditText();
signals:
    void sendFindData(const QString&,const QString&);


private:
    Ui::MainWindow *ui;
    ClientThread *m_threadSocket;
};

#endif // MAINWINDOW_H
