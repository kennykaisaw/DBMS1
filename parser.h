#ifndef PARSER_H
#define PARSER_H
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>

#include"db.h"
#include"table.h"

class Parser
{
public:
    bool create_database(QString text);
    bool show_database(QString text);
    bool select_database(QString text);
    bool drop_database(QString text);
    bool use_database(QString text);
    bool create_table(QString text);
    bool insert_table(QString text);
    bool delete_table(QString text);
    bool update_table(QString text);
    bool alter_table(QString text);
    bool select_from(QString text);
    string singlecolumn_constraints(QString text);
    string multicolumn_constraints(QString text);
     QString processColumnDefinition(const QString &tableDefinition, bool& isPrimaryKey, string& forignKeyName, string &forignKeyTable, string &default_content);
public:
    Parser(DB *db);
    Parser();
    QString parserfirst(QString text);
    QString parserSecond();
    bool parsermulty(QString text);


    DB *db;
};

#endif // PARSER_H
