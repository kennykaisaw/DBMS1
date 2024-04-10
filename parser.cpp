#include "parser.h"

Parser::Parser()
{

}

QString Parser::parserfirst( QString text)
{

    // Create a regular expression pattern to match
    QRegularExpression regex_create("create.+database.+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_drop("\\bdrop\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_insert("\\binsert\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_delete("\\bdelete\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_update("\\bupdate\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_alter("\\balter\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_show("\\bshow\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_grant("\\bgrant\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_use("\\buse\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_select("\\bselect\\b", QRegularExpression::CaseInsensitiveOption);









        //QString text = "Create database something;, cReate anything, but don't create chaos.";




     QRegularExpressionMatch match = regex_create.match(text);

    if (match.hasMatch()) {
        // Retrieve the matched QString
        QString matchedString = match.captured();
        //获取create database dbname三个分开的单词
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString dbname = words.at(2);
        //去掉分号
        dbname.chop(1);
        //调用相应函数
        DB db;
        db.createUserdb(dbname);


        qDebug() << "Match found:" << matchedString;
    } else {
        qDebug() << "No match found.";
    }

    return "";
}
//QString parserCreate()
//{
//     QRegularExpression regex_select("\\bselect\\b", QRegularExpression::CaseInsensitiveOption);
//}
