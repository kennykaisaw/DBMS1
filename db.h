#ifndef DB_H
#define DB_H
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include"authorization.h"
class DB
{
public:
    DB();
    bool createRuanko();
    bool createUserdb(QString dbname);
    bool usedb(QString db);
    bool showdbs();
    bool selectdatabase();
    bool dropdb(QString dbname);

    QString SYSTEMfolderPath="D:/program/QT/DBMS/Ruanko";
    QString CurrentDbPath="D:\\program\\QT\\DBMS\\Ruanko" ;
    QStringList dbs;
    //当前用户
    string user_name;
    string db_name;


};

#endif // DB_H
