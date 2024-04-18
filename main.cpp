
#include "mainwindow.h"
#include <QApplication>
#include"db.h"
#include"parser.h"
#include"table.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    DB db;
    MainWindow w;
    Parser p;
    p.parserfirst("INSERT INTO employees (id, name, department, salary) VALUES (1, 'John Doe', 'IT', 50000);");
//    Table t;



    w.show();

    return a.exec();
}

