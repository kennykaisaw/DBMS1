#include "parser.h"

Parser::Parser()
{

}

QString Parser::parserfirst( QString text)
{

    QRegularExpression regex_create("create.+database.+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_drop("\\bdrop\\b", QRegularExpression::CaseInsensitiveOption);

    QRegularExpression regex_insertinto("INSERT\\s+INTO\\s+\\w+\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_createtable("CREATE\\s+TABLE\\s+(\\w+)\\s*\\(([^;]+)\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_update("\\bupdate\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_alter("\\balter\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_show("\\bshow\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_grant("\\bgrant\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_use("\\buse\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_select("\\bselect\\b", QRegularExpression::CaseInsensitiveOption);









        //QString text = "Create database something;, cReate anything, but don't create chaos.";




     QRegularExpressionMatch match_create = regex_create.match(text);

    if (match_create.hasMatch()) {
        // Retrieve the matched QString
        QString matchedString = match_create.captured();
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


   } else {
       qDebug() << "No match found.";
   }

   QRegularExpressionMatch match_createtable = regex_createtable.match(text);

     // Check if a match is found
     if (match_createtable.hasMatch()) {
         QString tableName = match_createtable.captured(1); // Captured group 1: table name
         qDebug() << "Table Name:" << tableName;

         QString columnDefinitions = match_createtable.captured(2); // Captured group 2: column definitions
         qDebug() << "Column Definitions:" << columnDefinitions;
         // match every string without the comma
         QRegularExpression regex("\\b[^,]+\\b");


             QRegularExpressionMatchIterator matchIterator = regex.globalMatch(columnDefinitions);


             while (matchIterator.hasNext()) {
                 QRegularExpressionMatch match = matchIterator.next();
                 QString matchedString = match.captured(0);
                 //get column name,type,constraint
                  QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
                  QString columnname = words.at(0);
                   QString type = words.at(1);

                 qDebug() << "Matched String:" << matchedString;
             }

     } else {
         qDebug() << "No match found.";
     }

    return "";
}

