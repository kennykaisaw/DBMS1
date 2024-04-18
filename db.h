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

    QString currentdb ;

};

#endif // DB_H
