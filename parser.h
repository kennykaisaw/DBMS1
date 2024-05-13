#ifndef PARSER_H
#define PARSER_H
#include <QDebug>
#include <QRegularExpression>
#include"db.h"
#include"table.h"

class Parser
{
public:
    Parser(DB *db);
    Parser();
    QString parserfirst(QString text);
    QString parserSecond();
    bool parsermulty(QString text);


    DB *db;
};

#endif // PARSER_H
