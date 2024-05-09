#include "parser.h"

Parser::Parser()
{

}

QString Parser::parserfirst( QString text)
{

    QRegularExpression regex_create("\\bcreate\\s+database\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_drop("\\bdrop\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_delete("\\bdelete\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_insertinto("\\bINSERT\\s+INTO\\s+\\w+\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_createtable("\\bCREATE\\s+TABLE\\s+(\\w+)\\s*\\(([^;]+)\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_update("\\bupdate\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_alter("\\balter\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_show("\\bshow\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_grant("\\bgrant\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_use("\\buse\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_select("\\bselect\\s+\\S+\\s+from\\s+\\w+.*;", QRegularExpression::CaseInsensitiveOption);













     QRegularExpressionMatch match_create = regex_create.match(text);

    if (match_create.hasMatch())
    {
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
    }


    QRegularExpressionMatch match_drop = regex_drop.match(text);
   if (match_drop.hasMatch())
   {
       // Retrieve the matched QString
       QString matchedString = match_drop.captured();
       //获取create database dbname三个分开的单词
       QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
       QString tablename = words.at(1);
       //去掉分号
       tablename.chop(1);
       //调用相应函数
       string tname = tablename.toStdString();
       Table a(tname);
       a.dropTable();



       qDebug() << "Match found:" << matchedString;
   }


   QRegularExpressionMatch match_delete = regex_delete.match(text);
  if (match_delete.hasMatch()) {
      // Retrieve the matched QString
      QString matchedString = match_delete.captured();
      //获取create database dbname三个分开的单词
      QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      QString tablename = words.at(1);
      //去掉分号
      tablename.chop(1);
      //调用相应函数
      string tname = tablename.toStdString();
      Table a(tname);
      a.deleteFromTable();



      qDebug() << "Match found:" << matchedString;
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
                string tname = tablename.toStdString();
                Table a(tname);
                string attri = attribute[0].toStdString();
                string data =  attribute[1].toStdString();
                a.instertTOTable(data,attri);





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




            vector<tableRows>a;
            string tname = tableName.toStdString();




             while (matchIterator.hasNext()) {
                 QRegularExpressionMatch match = matchIterator.next();
                 QString matchedString = match.captured(0);
                 //get column name,type,constraint
                  QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);

                  QString columnname = words.at(0);
                  std::string col=columnname.toStdString();
                   QString type = words.at(1);
                        //////////////////////////////////////
                   a.push_back({col,type::VARCHAR,2,constrain::NONE,false,""});
                  // string tname = tableName.toStdString();
////////////////////////////////////////////////////

                 qDebug() << "Matched String:" << matchedString;
             }
             Table d(a,tname);
             //qDebug(a)
             //Table a1(a,tname);
//             for(auto aaaa:a)
//             {
//                 aaaa.rowName;
//                 QString qstr = QString::fromStdString(aaaa.rowName);
//                 qDebug()<<qstr;
//             }

     }

     QRegularExpressionMatch match_select = regex_select.match(text);

    if (match_select.hasMatch())
    {
        //select aas,sds,asf from table where a = b;
        QString matchedString = match_select.captured();
        //获取分开的单词
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString secondpos = words.at(1);
        QString fourthpos = words.at(3);
        QString fifthpos = words.at(4);
        //看第二个位置
        if(secondpos == "*")
        {

        }
        else
        {
            //逗号分割
            QStringList columnList = secondpos.split(",");
            //重复工作
            QString column_array[columnList.size()];
            for (int i=0;i<columnList.size();i++) {
                  column_array[i] = columnList[i];
               }
        }




        qDebug() << "Match found:" << matchedString;
    }else
    {
         qDebug() << "Match found:not";
    }

    return "";
}

