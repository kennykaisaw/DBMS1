#ifndef PARSER_H
#define PARSER_H
#include <QDebug>
#include <QRegularExpression>
#include"db.h"
#include"table.h"

class Parser
{
public:
    Parser();
    QString parserfirst(QString text);
    QString parserSecond();
};

#endif // PARSER_H
