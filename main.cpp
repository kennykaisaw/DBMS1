
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
    //p.parserfirst("INSERT INTO employees (id, name, department, salary) VALUES (1, John Doe, IT, 50000);");
    p.parserfirst("CREATE TABLE student ("
                  "    Sno VARCHAR(8) PRIMARY KEY,"
                  "    Sname VARCHAR(8) NOT NULL,"
                  "    Ssex NVARCHAR(2) CHECK (Ssex IN ('男', '女')),"
                  "    Sbirthday DATE,"
                  "    Classno VARCHAR(6),"
                  "    Totalcredit SMALLINT DEFAULT 0,"
                  "    Createdby VARCHAR(8) DEFAULT NULL,"
                  "    FOREIGN KEY (Classno) REFERENCES class(Classno)"
                  ");");
    //Table("student");
    QString ab = "CREATE TABLE student ("
                "    Sno VARCHAR(8) PRIMARY KEY,"
                "    Sname VARCHAR(8) NOT NULL,"
                "    Ssex NVARCHAR(2) CHECK (Ssex IN ('男', '女')),"
                "    Sbirthday DATE,"
                "    Classno VARCHAR(6),"
                "    Totalcredit SMALLINT DEFAULT 0,"
                "    Createdby VARCHAR(8) DEFAULT NULL,"
                "    FOREIGN KEY (Classno) REFERENCES class(Classno)"
                ");";


    //w.show();

    return a.exec();
}
