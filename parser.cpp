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


    //insertinto
      QRegularExpressionMatch match_insertinto= regex_insertinto.match(text);

     if (match_insertinto.hasMatch()) {

         QString matchedString = match_insertinto.captured();
         QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
         //get table name
         QString tablename = words[2];
          QRegularExpression regex_presenthesis("\\([^\\)]+\\)");
          QRegularExpressionMatchIterator matchIterator = regex_presenthesis.globalMatch(matchedString);

          QString attribute[2];

          if  (!matchIterator.hasNext())
          {
               qDebug() << "error" ;
          }
              for(int i=0;i<2;i++)
           {
               QRegularExpressionMatch match = matchIterator.next();
                       QString matchedText = match.captured(0);
                       matchedText.remove(0,1);
                       matchedText.remove(matchedText.length()-1,1);
                       //1, 'John Doe', 'IT', 50000 get every word without ,

                       QRegularExpression regex("\\b\\w+\\b");
                       QRegularExpressionMatchIterator matchIterator = regex.globalMatch(matchedText);
                       while (matchIterator.hasNext()) {
                               QRegularExpressionMatch match = matchIterator.next();
                               QString word = match.captured(0);
                               attribute[i].append(word);
                               attribute[i].append("|");
                           }


           }
              attribute[0].remove(attribute[0].size()-1,1);
               attribute[1].remove(attribute[1].size()-1,1);
                qDebug() <<tablename;
               qDebug() <<attribute[0];
                qDebug() <<attribute[1];






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

