#include "mainwindow.h"
#include <QApplication>
#include"db.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    DB db;
    MainWindow w;

    w.show();

    return a.exec();
}
