#ifndef DB_H
#define DB_H
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include"table.h"


class DB
{
public:
    DB();
    bool createRuanko();
    bool createUserdb(QString dbname);
    bool usedb(QString db);
};

#endif // DB_H
