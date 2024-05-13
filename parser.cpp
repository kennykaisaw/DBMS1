#include "parser.h"

Parser::Parser(DB *db):db(db)
{

}
Parser::Parser()
{

}


 bool Parser::parsermulty(QString text)
 {
     //匹配;前的内容
      QRegularExpression regex_delimeter(".+;", QRegularExpression::CaseInsensitiveOption);

       QRegularExpressionMatchIterator matchIterator = regex_delimeter.globalMatch(text);
       while (matchIterator.hasNext())
       {
           QRegularExpressionMatch match = matchIterator.next();
            QString matchedText = match.captured(0);
            parserfirst(matchedText);

       }
       return true;

 }
QString Parser::parserfirst(QString text)
{

    QRegularExpression regex_createdatabase("\\bcreate\\s+database\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_showdatabases("\\bshow\\s+databases;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_selectdatabase("\\bselect\\s+database\\(\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_use("\\buse\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_drop("\\bdrop\\s+\\w+\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_delete("\\bdelete\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_insertinto("\\bINSERT\\s+INTO\\s+\\w+\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_createtable("\\bCREATE\\s+TABLE\\s+(\\w+)\\s*\\(([^;]+)\\);", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_update("\\bupdate\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_alter("\\balter\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_show("\\bshow\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_grant("\\bgrant\\b", QRegularExpression::CaseInsensitiveOption);
    QRegularExpression regex_select("\\bselect\\s+\\S+\\s+from\\s+\\w+.*;", QRegularExpression::CaseInsensitiveOption);












    //create database dbname;
    QRegularExpressionMatch match_create = regex_createdatabase.match(text);

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

        this->db->createUserdb(dbname);


        qDebug() << "Match found:" << matchedString;
    }

    //showdatabases;
    QRegularExpressionMatch match_showdatabases = regex_showdatabases.match(text);

    if (match_showdatabases.hasMatch())
    {
         QString matchedString = match_showdatabases.captured();
        qDebug() << "Match found:" << matchedString;
        db->showdbs();
    }
    //selectdatabase();
    QRegularExpressionMatch match_selectdatabase = regex_selectdatabase.match(text);

    if (match_selectdatabase.hasMatch())
    {
         QString matchedString = match_selectdatabase.captured();
        qDebug() << "Match found:" << matchedString;
       db->selectdatabase();
    }
    //use dbname;
    QRegularExpressionMatch match_use = regex_use.match(text);

    if (match_use.hasMatch())
    {

        QString matchedString = match_use.captured();
        qDebug() << "Match found:" << matchedString;
        //获取use dbname;分开的单词
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString dbname = words.at(1);
        dbname.chop(1);
        db->usedb(dbname);
    }


    //drop;
    QRegularExpressionMatch match_drop = regex_drop.match(text);
    if (match_drop.hasMatch())
    {
        // Retrieve the matched QString
        QString matchedString = match_drop.captured();
        //获取drop database/table name三个分开的单词
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);        
        QString type = words.at(1);
        if(type == "database")
        {
             QString dbname = words.at(2);
             //去掉分号
             dbname.chop(1);
             //调用dropdb函数
             db->dropdb(dbname);
        }
        if(type == "table")
        {
             QString tablename = words.at(2);
             //去掉分号
             tablename.chop(1);
             //调用相应函数dropTable
             string tname = tablename.toStdString();
             Table a(tname,db);
             a.dropTable();
        }



        qDebug() << "Match found:" << matchedString;
    }

    //delete;
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
        Table a(tname,db);
        a.deleteFromTable();



        qDebug() << "Match found:" << matchedString;
    }



    //insert into t1 (sno,sname) values(12,22);
    QRegularExpressionMatch match_insertinto = regex_insertinto.match(text);

    if (match_insertinto.hasMatch())
    {
        QString matchedString = match_insertinto.captured();
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);        
        QString tablename = words[2];
        //剔除括号
        QRegularExpression regex_presenthesis("\\([^\\)]+\\)");
        QRegularExpressionMatchIterator matchIterator = regex_presenthesis.globalMatch(matchedString);
        QString attribute[2];
        if (!matchIterator.hasNext())
        {
            qDebug() << "error";
        }
        for (int i = 0; i < 2; i++)
        {
            QRegularExpressionMatch match = matchIterator.next();
            QString matchedText = match.captured(0);
            matchedText.remove(0, 1);
            matchedText.remove(matchedText.length() - 1, 1);

            //
            QRegularExpression regex("\\b\\w+\\b");
            QRegularExpressionMatchIterator matchIterator = regex.globalMatch(matchedText);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                QString word = match.captured(0);
                attribute[i].append(word);
                attribute[i].append("|");
            }


        }
        attribute[0].remove(attribute[0].size() - 1, 1);
        attribute[1].remove(attribute[1].size() - 1, 1);
        qDebug() << tablename;
        qDebug() << attribute[0];
        qDebug() << attribute[1];
        string tname = tablename.toStdString();
        Table a(tname,db);
        string attri = attribute[0].toStdString();
        string data = attribute[1].toStdString();
        a.instertTOTable(data, attri);





    }
    //createtable(a1 int,a2,int);
    QRegularExpressionMatch match_createtable = regex_createtable.match(text);    
    if (match_createtable.hasMatch()) {
        QString tableName = match_createtable.captured(1);
        string tname = tableName.toStdString();
        QString columnDefinitions = match_createtable.captured(2);
        // 通过逗号分割
        QRegularExpression regex("\\b[^,]+\\b");
        QRegularExpressionMatchIterator matchIterator = regex.globalMatch(columnDefinitions);
        //储存列
        vector<tableRows>a;

        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            QString matchedString = match.captured(0);
            //get column name,type,constraint
            QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);

            QString columnname = words.at(0);
            std::string col = columnname.toStdString();
            QString type = words.at(1);
            //////////////////////////////////////
            //a.push_back({ col,type::INT,2,constrain::NONE,false,"" });
            // string tname = tableName.toStdString();
////////////////////////////////////////////////////

            qDebug() << "Matched String:" << matchedString;
        }
        Table d(a, tname,db);


    }

    // select aas,sds,asf from table /where a = b;
    QRegularExpressionMatch match_select = regex_select.match(text);
    if (match_select.hasMatch())
    {

        QString matchedString = match_select.captured();
        //获取分开的单词
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString secondpos = words.at(1);
        QString fourthpos = words.at(3);

        //看第二个位置
        if (secondpos == "*")
        {

        }
        else
        {
            //逗号分割
            QStringList columnList = secondpos.split(",");
            //重复工作qstring改string
            int size = columnList.size();
            vector <string> column_array(size);
            for (int i = 0; i < size; i++) {
                column_array[i] = columnList[i].toStdString();
            }
            //看第四个位置;或第五个位置为where，或者嵌套查询
            if(fourthpos.back() == ';')
            {
                fourthpos.chop(1);
                string tablename =fourthpos.toStdString();
                Table tb(tablename,db);
                tb.selectfrom(column_array,tablename);
            }

        }








        qDebug() << "Match found:" << matchedString;
    }
    else
    {
        //qDebug() << "Match found:not";
    }

    return "";
}

