#include "mainwindow.h"
#include <QApplication>
#include"db.h"
#include"parser.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    DB db;
    MainWindow w;
    Parser p;
    p.parserfirst("create database test1db;");


    w.show();

    return a.exec();
}
