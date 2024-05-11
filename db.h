#ifndef DB_H
#define DB_H
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

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

};

#endif // DB_H
