
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
//    Parser p;
//    p.parserfirst("INSERT INTO student (sname, ssex, sno) VALUES (1,John Doe,sii);");
//    Table aaaa("student");
//    aaaa.show();
     //p.parserfirst("delete student;");
//    p.parserfirst("CREATE TABLE student1 ("
//                  "    Sno INT,"
//                  "sname INT);"

//                  );
//    //Table("student");
//    QString ab = "CREATE TABLE student ("
//                "    Sno VARCHAR(8) PRIMARY KEY,"
//                "    Sname VARCHAR(8) NOT NULL,"
//                "    Ssex NVARCHAR(2) CHECK (Ssex IN ('男', '女')),"
//                "    Sbirthday DATE,"
//                "    Classno VARCHAR(6),"
//                "    Totalcredit SMALLINT DEFAULT 0,"
//                "    Createdby VARCHAR(8) DEFAULT NULL,"
//                "    FOREIGN KEY (Classno) REFERENCES class(Classno)"
//                ");";


    w.show();

    return a.exec();
    //Table aa("student");
}
