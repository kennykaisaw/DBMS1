#ifndef PARSER_H
#define PARSER_H
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <utility>
#include"db.h"
#include"table.h"
#include"authorization.h"

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
    pair<vector<string>, vector<string>> multicolumn_constraints(QString text);
    QString processColumnDefinition(const QString &tableDefinition, bool& isPrimaryKey, string& forignKeyName, string &forignKeyTable, string &default_content);
    bool login(QString  name,QString password);
     bool register_user(QString  name,QString password);
     bool grant(QString text);


public:
    Parser(DB *db);
    Parser();
    QString parserfirst(QString text);
    bool parserAndCheck(QString text);
    bool parsermulty(QString text);
    string user = "root";

    DB *db;
};

#endif // PARSER_H
