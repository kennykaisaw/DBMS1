
#include "mainwindow.h"
#include <QApplication>
#include"db.h"
#include"parser.h"
#include"table.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);



    MainWindow w;
    w.show();



    return a.exec();
    //Table aa("student");
}
