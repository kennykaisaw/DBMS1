#include "parser.h"

Parser::Parser()
{

}

QString Parser::parserfirst( QString text)
{

    QRegularExpression regex_create("create.+database.+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_drop("\\bdrop\\b", QRegularExpression::CaseInsensitiveOption);

    QRegularExpression regex_insertinto("INSERT\\s+INTO\\s+\\w+\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_delete("\\bdelete\\b", QRegularExpression::CaseInsensitiveOption);
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

    QRegularExpressionMatch match_insertinto= regex_insertinto.match(text);

   if (match_insertinto.hasMatch()) {
       // Retrieve the matched QString
       QString matchedString = match_insertinto.captured();
       QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//       for (const QString& word : words) {
//              qDebug() << word<<endl;
//          }
        QRegularExpression regex_presenthesis("\\([^\\)]+\\)");
        QRegularExpressionMatchIterator matchIterator = regex_presenthesis.globalMatch("INSERT INTO employees (id, name, department, salary) VALUES (1, 'John Doe', 'IT', 50000);");
         while  (matchIterator.hasNext())
         {
             QRegularExpressionMatch match = matchIterator.next();
                     QString matchedText = match.captured(0); // Captured group 0: entire matched substring
                     qDebug() << "Match found:" << matchedText;
         }




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
