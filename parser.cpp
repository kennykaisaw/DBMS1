
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

//    QRegularExpression regex_createdatabase("\\bcreate\\s+database\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_showdatabases("\\bshow\\s+databases;", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_selectdatabase("\\bselect\\s+database\\(\\);", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_use("\\buse\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_drop("\\bdrop\\s+\\w+\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_delete("\\bdelete\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_insertinto("\\bINSERT\\s+INTO\\s+\\w+\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_createtable("\\bCREATE\\s+TABLE\\s+(\\w+)\\s*\\(([^;]+)\\);", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_update("\\bupdate\\b", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_alter("\\balter\\b", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_show("\\bshow\\b", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_grant("\\bgrant\\b", QRegularExpression::CaseInsensitiveOption);
//    QRegularExpression regex_select("\\bselect\\s+\\S+\\s+from\\s+\\w+.*;", QRegularExpression::CaseInsensitiveOption);


//     create_database( text);
//     show_database( text);
//     select_database( text);
//     drop_database( text);
//     use_database( text);
//     create_table( text);
//     insert_table( text);
//     delete_table( text);
//     update_table( text);
//     alter_table( text);
//     select_from( text);
     singlecolumn_constraints( text);
//     multicolumn_constraints( text);


     return "";
}




 bool Parser::create_database(QString text)
 {
     QRegularExpression regex_createdatabase("\\bcreate\\s+database\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
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



    }
    return true;
 }

    //showdatabases;
 bool  Parser::show_database(QString text)
{
     QRegularExpression regex_showdatabases("\\bshow\\s+databases;", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match_showdatabases = regex_showdatabases.match(text);

    if (match_showdatabases.hasMatch())
    {
         QString matchedString = match_showdatabases.captured();

        db->showdbs();
    }
     return true;
 }

    //selectdatabase();
 bool  Parser::select_database(QString text)
 {

     QRegularExpression regex_selectdatabase("\\bselect\\s+database\\(\\);", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match_selectdatabase = regex_selectdatabase.match(text);

    if (match_selectdatabase.hasMatch())
    {
         QString matchedString = match_selectdatabase.captured();
        qDebug() << "Match found:" << matchedString;
       db->selectdatabase();
    }
     return true;
 }
    //use dbname;
 bool  Parser::use_database(QString text)
 {

     QRegularExpression regex_use("\\buse\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match_use = regex_use.match(text);

    if (match_use.hasMatch())
    {

        QString matchedString = match_use.captured();

        //获取use dbname;分开的单词
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString dbname = words.at(1);
        dbname.chop(1);
        db->usedb(dbname);
    }
     return true;

}
    //drop;
 bool  Parser::drop_database(QString text)
 {
     QRegularExpression regex_drop("\\bdrop\\s+\\w+\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
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
             //Table a(tname,db);
             //a.dropTable();
        }




    }
    return true;
}


 bool Parser::delete_table(QString text)
{
      QRegularExpression regex_delete("\\bdelete\\s+\\w+;", QRegularExpression::CaseInsensitiveOption);
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




        qDebug() << "Match found:" << matchedString;
    }
 }



    //insert into t1 (sno,sname) values(12,22);
 bool Parser::insert_table(QString text)
 {
      QRegularExpression regex_insertinto("\\bINSERT\\s+INTO\\s+\\w+\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption);
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

        string attri = attribute[0].toStdString();
        string data = attribute[1].toStdString();




 return true;
    }
 }
// CREATE TABLE checklists(
//   id INT,
//   task_id INT,
//   title VARCHAR(255) NOT NULL,
//   is_completed BOOLEAN NOT NULL DEFAULT FALSE,
//   PRIMARY KEY (id, task_id),
//   FOREIGN KEY (task_id)
//       REFERENCES tasks (id)
//       ON UPDATE RESTRICT
//       ON DELETE CASCADE
// );
QString Parser::processColumnDefinition(const QString &columnDefinition) {
     // 正则表达式匹配列定义，识别主键和外键
     QRegularExpression primaryKeyRegex(R"(\bPRIMARY\s+KEY\b)");
     QRegularExpression foreignKeyRegex(R"(\bREFERENCES\s+\w+\s*\(.*\))");

     QString processedColumn = columnDefinition.trimmed();

     // 去掉主键定义
     if (primaryKeyRegex.match(processedColumn).hasMatch()) {
         processedColumn.remove(primaryKeyRegex);
         qDebug() << "Column with PRIMARY KEY:" << processedColumn.trimmed();
     }
     // 去掉外键定义
     else if (foreignKeyRegex.match(processedColumn).hasMatch()) {
         processedColumn.remove(foreignKeyRegex);
         qDebug() << "Column with FOREIGN KEY:" << processedColumn.trimmed();
     } else {
         qDebug() << "Regular Column:" << processedColumn.trimmed();
     }
     return   processedColumn.trimmed();
 }

 bool Parser::create_table(QString text)
    {
     //
        QRegularExpression regex_createtable(
            R"(\bCREATE\s+TABLE\s+(\w+)\s*\((.*?)\);)",
            QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption
        );

     QRegularExpressionMatch match_createtable = regex_createtable.match(text);
     if (match_createtable.hasMatch())
     {
         QString tableName = match_createtable.captured(1);
         string tname = tableName.toStdString();
         //括号内容
         QString columnsAndConstraints = match_createtable.captured(2);


         qDebug() << "Table Name:" << tableName;
         qDebug() << "column:" << columnsAndConstraints;
         // 通过逗号分割括号内每行内容
         // 分离列定义、主键和外键
         QRegularExpression primaryKeyRegex(R"(\bPRIMARY\s+KEY\s*\(([^)]+)\))");
         QRegularExpression foreignKeyRegex(R"(\bFOREIGN\s+KEY\s*\(([^)]+)\)\s*REFERENCES\s+(\w+)\s*\(([^)]+)\)(?:\s*ON\s+UPDATE\s+(\w+)\s*ON\s+DELETE\s+(\w+))?)");

         // 提取主键定义
         QRegularExpressionMatch primaryKeyMatch = primaryKeyRegex.match(columnsAndConstraints);
         QString primaryKey;
         if (primaryKeyMatch.hasMatch()) {
             primaryKey = primaryKeyMatch.captured(0);
             columnsAndConstraints.remove(primaryKey);
         }

         // 提取外键定义
         QStringList foreignKeys;
         QRegularExpressionMatchIterator foreignKeyIterator = foreignKeyRegex.globalMatch(columnsAndConstraints);
         while (foreignKeyIterator.hasNext()) {
             QRegularExpressionMatch foreignKeyMatch = foreignKeyIterator.next();
             QString foreignKey = foreignKeyMatch.captured(0);
             foreignKeys.append(foreignKey);
             columnsAndConstraints.remove(foreignKey);
         }

         // 清理和分离列定义,通过空白格和逗号和空白格分离
         QStringList columnsList = columnsAndConstraints.split(QRegularExpression("\\s*,\\s*"), Qt::SkipEmptyParts);

         qDebug() << "Columns and Constraints:";
         for (const QString &column : columnsList) {
             qDebug() << column.trimmed();
         }

         if (!primaryKey.isEmpty()) {
             qDebug() << "Primary Key:" << primaryKey.trimmed();
         }

         qDebug() << "Foreign Keys:";
         for (const QString &foreignKey : foreignKeys) {
             qDebug() << foreignKey.trimmed();
         }
         //处理每一列
         for (const QString &column : columnsList) {
            QString trimpriaryandforiegncolumn = processColumnDefinition(column);
            // 正则表达式匹配列定义并捕获列名、数据类型和约束
               QRegularExpression regex(R"((\w+)\s+(\w+.*?)(?:\s+(.*))?$)");

               QRegularExpressionMatch match = regex.match(trimpriaryandforiegncolumn.trimmed());

               if (match.hasMatch()) {
                   QString columnName = match.captured(1).trimmed();
                   QString columnType = match.captured(2).trimmed();
                   QString constraints = match.captured(3).trimmed();


                   qDebug() << "Column Name:" << columnName;
                   qDebug() << "Column Type and Constraints:" << columnType;
                   qDebug() << "Remaining Constraints:" << constraints;
               } else {
                   qDebug() << "Invalid column definition:" << trimpriaryandforiegncolumn.trimmed();
               }
         }
     }


     else {
         qDebug() << "No match found.";

     }





     return true;
    }
 bool Parser::select_from(QString text)
  {
      QRegularExpression regex_select("\\bselect\\s+\\S+\\s+from\\s+\\w+.*;", QRegularExpression::CaseInsensitiveOption);

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

               // tb.selectfrom(column_array,tablename);
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
 string  Parser::singlecolumn_constraints(QString text)
 {
     string res = "";
     // 使用正则表达式匹配or和and条件，并将它们分离开来
        QRegularExpression regex_divide("\\b(or|and)\\b(?!.*\\bbetween\\b)", QRegularExpression::CaseInsensitiveOption);
       //知道是or还是and分离
        QRegularExpressionMatchIterator matchIterator = regex_divide.globalMatch(text);

           int lastPosition = 0;
           while (matchIterator.hasNext())
           {
               res.append("*");
               QRegularExpressionMatch match = matchIterator.next();
               QString separator = match.captured(1);
              if(separator.toLower() == "and")
              {
                  res.append("A");
              }
              if(separator.toLower() == "or")
              {
                  res.append("O");
              }
           }

           QString lastCondition = text.mid(lastPosition).trimmed();
           res.append("*");
//           qDebug() << "Condition:" << lastCondition << "Separator: None";
       //通过and和or分离
         QStringList conditions = text.split(regex_divide, QString::SkipEmptyParts);

         res = to_string(conditions.size())+"\n" + res;
        qDebug() << "Separated Conditions:";
        for (const QString &cond : conditions)
        {
             QRegularExpression regex_opperand("\\b(=|>|>=|<=|\\w+)\\b", QRegularExpression::CaseInsensitiveOption);
               QRegularExpressionMatch match = regex_opperand.match(cond.trimmed());
                QString matchedopperand = match.captured(1);


            qDebug() << cond.trimmed();
        }
        return res;
 }


