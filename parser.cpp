
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
 bool Parser::parserAndCheck(QString text)
 {
     QString permission_type = parserfirst(text);
     authorization au;
     if(au.search_permission(permission_type.toStdString(),db->db_name)==false)
     {
         //撤回
         cerr<<"无权限"<<endl;
         return false;
     }
     return true;
 }
QString Parser::parserfirst(QString text)
{


    if(create_database( text))
    {
         return "create_database";
    }
    if(show_database( text))
    {
         return "show_database";
    }
    if(select_database( text))
    {
         return "select_database";
    }
    if(drop_database( text))
    {
         return "drop_database";
    }
    if(use_database( text))
    {
         return "use_database";
    }
    if(create_table( text))
    {
         return "create_table";
    }
    if(insert_table( text))
    {
         return "insert_table";
    }
    if(delete_table( text))
    {
         return "delete_table";
    }
    if(update_table( text))
    {
         return "update_table";
    }
    if(alter_table( text))
    {
         return "alter_table";
    }
    if(select_from( text))
    {
         return "select_from";
    }

    //    create_database( text);
    //     show_database( text);
    //     select_database( text);
        // drop_database( text);
    //     use_database( text);
    //     create_table( text);
    //     insert_table( text);
    //     delete_table( text);
    //     update_table( text);
    //     alter_table( text);
    //select_from( text);
    //    singlecolumn_constraints( text);
    //    multicolumn_constraints( text);


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
    string tablename;
    Table::delete_mode mode_d;//是否带where的删除
    vector<string>rowname_d;//列名
    vector<string>constrainMessage_d;//限制信息
    string operation_d;//表达式
    QRegularExpression regex("delete\\s+from\\s+(\\w+)\\s*(?:where\\s+(.*?);)?", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = regex.match(text);

    if (match.hasMatch()) {
        QString tableName = match.captured(1);
        QString whereClause = match.captured(2);
        if (whereClause.isEmpty())
        {
            mode_d =Table::delete_mode::ALL;
        }
        if (!whereClause.isEmpty())
        {
            mode_d =Table::delete_mode::SELECT;
        }



        qDebug() << "Table Name:" << tableName;
        qDebug() << "Conditions:" << whereClause;
        tablename = tableName.toStdString();
        rowname_d = multicolumn_constraints(whereClause).first;
        constrainMessage_d=multicolumn_constraints(whereClause).second;
        operation_d = constrainMessage_d.back();
        constrainMessage_d.pop_back();
        Table c(tablename,db);
        c.deleteFromTable(mode_d,rowname_d,constrainMessage_d,operation_d);
    }
    return true;
}



//insert into t1 (sno,sname) values('s1',22);
bool Parser::insert_table(QString text)
{
    QRegularExpression regex_insertinto("\\bINSERT\\s+INTO\\s+(\\w+)\\s*\\([^\\)]+\\)\\s*VALUES\\s*\\([^\\)]+\\);", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match_insertinto = regex_insertinto.match(text);

    if (match_insertinto.hasMatch())
    {
        QString matchedString = match_insertinto.captured();
        QStringList words = matchedString.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QString tablename = match_insertinto.captured(1);
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
            QRegularExpression regex("\\b\\w+\\b|'([^']+)'|\\b\\d+\\b");
            QRegularExpressionMatchIterator matchIterator = regex.globalMatch(matchedText);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                QString word = match.captured(0);
                qDebug()<<"value:"<<word;
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
QString Parser::processColumnDefinition(const QString &columnDefinition, bool& isPrimaryKey, string &forignKeyName, string &forignKeyTable, string &default_content) {
    // 正则表达式匹配列定义，识别主键和外键
    QRegularExpression primaryKeyRegex(R"(\bPRIMARY\s+KEY\b)");
    QRegularExpression foreignKeyRegex(R"(\bREFERENCES\s+(\w+)\s*\((.*)\))");
    QRegularExpression defaultvalueRegex(R"(\bDEFAULT\s+(.*)\s+\b)", QRegularExpression::CaseInsensitiveOption);
    QString processedColumn = columnDefinition.trimmed();

    // 去掉主键定义
    if (primaryKeyRegex.match(processedColumn).hasMatch()) {
        isPrimaryKey = true;
        processedColumn.remove(primaryKeyRegex);
        qDebug() << "Column with PRIMARY KEY:" << processedColumn.trimmed();
    }
    // 去掉默认定义
    if (defaultvalueRegex.match(processedColumn).hasMatch()) {
        default_content = foreignKeyRegex.match(processedColumn).captured(1).toStdString();

        processedColumn.remove(defaultvalueRegex);
        qDebug() << "Column with PRIMARY KEY:" << processedColumn.trimmed();
    }
    // 去掉外键定义
    else if (foreignKeyRegex.match(processedColumn).hasMatch()) {
        forignKeyTable = foreignKeyRegex.match(processedColumn).captured(1).toStdString();

        forignKeyName         = foreignKeyRegex.match(processedColumn).captured(2).toStdString();
        processedColumn.remove(foreignKeyRegex);
        qDebug() << "Column with FOREIGN KEY:" << processedColumn.trimmed();
    } else {
        qDebug() << "Regular Column:" << processedColumn.trimmed();
    }
    return   processedColumn.trimmed();
}

bool Parser::create_table(QString text)
{
    //要填入的内容
    string tablename;
    vector<tableRows> tableRows_array;
    vector<string> names_c;
    vector<string> cons_c ;

    QRegularExpression regex_createtable(
                R"(\bCREATE\s+TABLE\s+(\w+)\s*\((.*?)\);)",
                QRegularExpression::CaseInsensitiveOption | QRegularExpression::DotMatchesEverythingOption
                );

    QRegularExpressionMatch match_createtable = regex_createtable.match(text);
    if (match_createtable.hasMatch())
    {
        QString q_tableName = match_createtable.captured(1);
        string tablename = q_tableName.toStdString();
        //括号内容
        QString columnsAndConstraints = match_createtable.captured(2);


        qDebug() << "Table Name:" << q_tableName;
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
        for (const QString &column : columnsList)
        {
            //每一列要填入的东西
            //要填入的东西
            string rowName;//列名
            type rowType;//列的数据要求INT
            bool isPrimaryKey=false;//是否主键
            string forignKeyName="";//外键名称//多个外键怎么办？？？？？？？？？？？？？
            string forignKeyTable="";//外键所属表名称
            string default_content="";//默认值
            bool constrainMessage=false;//列的限制
            string content="";


            QString trimpriaryandforiegncolumn = processColumnDefinition(column,isPrimaryKey,forignKeyName,forignKeyTable,default_content);
            // 正则表达式匹配列定义并捕获列名、数据类型和约束
            QRegularExpression regex(R"((\w+)\s+(\w+.*?)(?:\s+(.*))?$)");

            QRegularExpressionMatch match = regex.match(trimpriaryandforiegncolumn.trimmed());

            if (match.hasMatch()) {
                QString columnName = match.captured(1).trimmed();
                QString columnType = match.captured(2).trimmed();
                QString constraints = match.captured(3).trimmed();

                //填入内容
                rowName = columnName.toStdString();
                if(columnType.toLower() == "int")
                {
                    rowType = INT;
                }
                if(columnType.toLower() == "double")
                {
                    rowType = DOUBLE;
                }
                if(columnType.toLower() == "char")
                {
                    rowType = CHAR;
                }
                //填入列定义
                tableRows_array.push_back(tableRows(rowName,rowType,isPrimaryKey,forignKeyName,forignKeyTable,default_content,constrainMessage,content));


                string str = singlecolumn_constraints(constraints);
                names_c.push_back(rowName);
                cons_c.push_back(str);



                qDebug() << "Column Name:" << columnName;
                qDebug() << "Column Type and Constraints:" << columnType;
                qDebug() << "Remaining Constraints:" << constraints;
            } else {
                qDebug() << "Invalid column definition:" << trimpriaryandforiegncolumn.trimmed();
            }
        }

        //建表
        Table table(tableRows_array,tablename,db);
        //添加限制
        for(unsigned int i=0;i<cons_c.size();++i)
        {
            table.setContrain(names_c[i],cons_c[i]);
        }
        //table.updateForeignKenyMessages();
    }




    else {
        qDebug() << "No match found.";

    }





    return true;
}
bool Parser::select_from(QString text)
{
    QRegularExpression regex_select("select\\s+(\\*|[\\w+\\s*,]+)\\s+from\\s+(\\w+)\\s*(;|(?:where\\s+(.*?);))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match_select = regex_select.match(text);
    if (match_select.hasMatch())
    {
        //括号里内容,即列名
        QString column_names = match_select.captured(1);
        //表名
        QString tablename = match_select.captured(2);
        //分号或where语句
        QString delimiterorwhere = match_select.captured(3);
        //where语句(不含where)，如果没有为空
        QString whereclause = match_select.captured(4);
        qDebug() << "column_names" << column_names;
        qDebug() << "tablename" << tablename;
        qDebug() << "delimiterorwhere" << delimiterorwhere;
        qDebug() << "whereclause" << whereclause;
        //通过,分离列名
        QStringList columns = column_names.split(QRegularExpression("\\s*,\\s*"), Qt::SkipEmptyParts);

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
    res.append("*\n");
    //           qDebug() << "Condition:" << lastCondition << "Separator: None";
    //通过and和or分离
    QStringList conditions = text.split(regex_divide, QString::SkipEmptyParts);

    res = to_string(conditions.size())+"\n" + res;
    qDebug() << "Separated Conditions:";
    for (const QString &cond : conditions)
    {
        QRegularExpression regex_opperand("(=|>|>=|<=|\\w+)", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = regex_opperand.match(cond.trimmed());
        QString matchedopperand = match.captured(1);
        if (!matchedopperand.isEmpty()) {
            qDebug() << "Operator:" << matchedopperand;
        }
        if(matchedopperand != "=")
        {


            if(matchedopperand == ">")
            {
                res.append("Dayu");
            }
            if(matchedopperand == ">=")
            {
                res.append("DayuDengyu");
            }
            if(matchedopperand == "<")
            {
                res.append("Xiaoyu");
            }
            if(matchedopperand == "<=")
            {
                res.append("XiaoyuDengyu");
            }

            if(matchedopperand == "notIn")
            {
                res.append("notIn");
            }
            if(matchedopperand == "in")
            {
                res.append("in");
            }
            if(matchedopperand == "notNull")
            {
                res.append("notNull");
            }
            if(matchedopperand == "between")
            {
                res.append("between");
            }
            QRegularExpression regex_value("\\d+");
            QRegularExpressionMatchIterator matchIterator = regex_value.globalMatch(cond);

            qDebug() << "Matched Numbers:";
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                QString matchedNumber = match.captured();
                res.append("|"+matchedNumber.toStdString());
            }
            res.append("\n");
        }

        if(matchedopperand == "=")
        {
            res.append("Dengyu");
            QRegularExpression regex_value("'([^']+)'|\\d+");
            QRegularExpressionMatchIterator matchIterator = regex_value.globalMatch(cond);

            qDebug() << "Matched Numbers:";
            while (matchIterator.hasNext())
            {
                QRegularExpressionMatch match = matchIterator.next();
                QString matchedText  = match.captured();
                res.append("|"+matchedText .toStdString());

            }
            res.append("\n");
        }






        qDebug() << cond.trimmed();
    }
    qDebug() <<QString::fromStdString(res);
    return res;
}
bool Parser::alter_table(QString text)
{
    QRegularExpression regex_alter(R"(alter\s+table\s+(\w+)\s+(drop|add|modify)\s+(.*?);)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = regex_alter.match(text);

    if (match.hasMatch())
    {
        QString tableName = match.captured(1);
        QString operationType = match.captured(2);
        QString remainingPart = match.captured(3).trimmed();

        qDebug() << "Table Name:" << tableName;
        qDebug() << "Operation Type:" << operationType;
        qDebug() << "Remaining Part:" << remainingPart;
        //通过,分离具体改变的列
        QStringList columns = remainingPart.split(QRegularExpression("\\s*,\\s*"), Qt::SkipEmptyParts);
        if(operationType.toLower() == "drop")
        {
            for (const QString &column : columns)
            {

                QRegularExpression regex_primarykey(R"(constraint\s+(\w+)\s+primary\s*key)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpression regex_foreignkey(R"(constraint\s+(\w+)\s+foreign\s*key)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                // QRegularExpression regex_constraint(R"((\w+)\s+(constraint\s+primary\s+key|constraint\s+foreign\s+key\s*references\s*\w+\(\w+\)|\s+(\w+)\s+(\w+))\s*(.*?))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpression regex_column(R"(column\s+(\w+)\s+)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpressionMatch regex_primarykey_match = regex_primarykey.match(column);
                QRegularExpressionMatch regex_foreignkey_match = regex_foreignkey.match(column);
                QRegularExpressionMatch regex_column_match = regex_column.match(column);

                //记得break
                if (regex_primarykey_match.hasMatch())
                {
                    QString column_name = match.captured(1);



                    break;
                }
                if (regex_foreignkey_match.hasMatch())
                {
                    QString column_name = match.captured(1);
                    QString foreign_table_name = match.captured(2);
                    QString foreign_column_name = match.captured(3);
                    qDebug() << "column_name"<<column_name <<endl;
                    qDebug() << "foreign_table_name"<<foreign_table_name <<endl;
                    qDebug() << "foreign_table_name"<<foreign_column_name <<endl;


                    break;
                }
                if (regex_column_match.hasMatch())
                {
                    QString column_name = match.captured(1);


                    break;
                }

            }
        }
        if(operationType.toLower() == "add")
        {

            for (const QString &column : columns)
            {


                QRegularExpression regex_primarykey(R"(constraint\s(\w+)primary\s*key)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpression regex_foreignkey(R"(constraint\s+foreign key\s*\(\w+\)\s*references\s*\w+\(\w+\))\s*(.*?);)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                // QRegularExpression regex_constraint(R"((\w+)\s+(constraint\s+primary\s+key|constraint\s+foreign\s+key\s*references\s*\w+\(\w+\)|\s+(\w+)\s+(\w+))\s*(.*?))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpression regex_column(R"(column\s+(\w+))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpressionMatch regex_primarykey_match = regex_primarykey.match(column);
                QRegularExpressionMatch regex_foreignkey_match = regex_foreignkey.match(column);
                QRegularExpressionMatch regex_column_match = regex_column.match(column);

                //记得break
                if (regex_primarykey_match.hasMatch())
                {
                    QString column_name = match.captured(1);



                    break;
                }
                if (regex_foreignkey_match.hasMatch())
                {
                    QString column_name = match.captured(1);
                    QString foreign_table_name = match.captured(2);
                    QString foreign_column_name = match.captured(3);
                    qDebug() << "column_name"<<column_name <<endl;
                    qDebug() << "foreign_table_name"<<foreign_table_name <<endl;
                    qDebug() << "foreign_table_name"<<foreign_column_name <<endl;


                    break;
                }
                if (regex_column_match.hasMatch())
                {
                    QString column_name = match.captured(1);
                    QString column_type = match.captured(2);

                    break;
                }

            }
        }
        if(operationType.toLower() == "modify")
        {
            for (const QString &column : columns)
            {


                QRegularExpression regex_type(R"(\(\s+(\w+)\s+(\w+)\s+\))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpression regex_default(R"((\w+)\s+default\s+(\w+))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                // QRegularExpression regex_constraint(R"((\w+)\s+(constraint\s+primary\s+key|constraint\s+foreign\s+key\s*references\s*\w+\(\w+\)|\s+(\w+)\s+(\w+))\s*(.*?))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                //QRegularExpression regex_column(R"(column\s+(\w+)\s+(\w+))", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
                QRegularExpressionMatch regex_type_match = regex_type.match(column);
                QRegularExpressionMatch regex_default_match = regex_default.match(column);
                //QRegularExpressionMatch regex_column_match = regex_column.match(column);

                //记得break
                if (regex_type_match.hasMatch())
                {
                    QString column_name = match.captured(1);



                    break;
                }
                if (regex_default_match.hasMatch())
                {
                    QString column_name = match.captured(1);
                    QString foreign_table_name = match.captured(2);
                    QString foreign_column_name = match.captured(3);
                    qDebug() << "column_name"<<column_name <<endl;
                    qDebug() << "foreign_table_name"<<foreign_table_name <<endl;
                    qDebug() << "foreign_table_name"<<foreign_column_name <<endl;


                    break;
                }


            }
        }
    }
}
bool Parser::update_table(QString text)
{
    // 匹配 UPDATE 语句的表名、SET 后面的内容和 WHERE 后面的内容
    QRegularExpression regex("UPDATE\\s+(\\w+)\\s+SET\\s+(.*?)\\s+WHERE\\s+(.*)", QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match = regex.match(text);
    if (match.hasMatch())
    {
        QString tableName = match.captured(1);
        QString setClause = match.captured(2);
        QString whereClause = match.captured(3);

        //set部分解析
        // 逗号分割每一行
        QStringList set_lines = setClause.split(",", Qt::SkipEmptyParts);

        // 解析每一行，提取列名和对应的值
        foreach (const QString &line, set_lines)
        {
            QRegularExpression regex("\\b(\\w+)\\s*=\\s*('([^']*)'\\s*");
            QRegularExpressionMatch match = regex.match(line);
            if (match.hasMatch())
            {
                QString columnName = match.captured(1);
                QString value = match.captured(2);
                qDebug() << "Column Name:" << columnName.trimmed();
                qDebug() << "Value:" << value.trimmed();

            }
        }


        qDebug() << "Table Name:" << tableName;
        qDebug() << "SET Clause:" << setClause;
        qDebug() << "WHERE Clause:" << whereClause;
    }
}
pair<vector<string>, vector<string>> Parser::multicolumn_constraints(QString text)
{
    vector<string> names_array;
    vector<string> cons_string_array ;
    pair<vector<string>, vector<string>> pair;
    //names_constraint_array
    string expression_res;
    // 使用正则表达式匹配or和and条件，并将它们分离开来
    QRegularExpression regex_divide("\\b(or|and)\\b(?!.*\\bbetween\\b)", QRegularExpression::CaseInsensitiveOption);
    //知道是or还是and分离
    QRegularExpressionMatchIterator matchIterator = regex_divide.globalMatch(text);

    int lastPosition = 0;
    while (matchIterator.hasNext())
    {
        //res.append("*");
        QRegularExpressionMatch match = matchIterator.next();
        QString separator = match.captured(1);
        if(separator.toLower() == "and")
        {
            expression_res.append("A");
        }
        if(separator.toLower() == "or")
        {
            expression_res.append("O");
        }
    }

    QString lastCondition = text.mid(lastPosition).trimmed();
    expression_res.append("*\n");
    //           qDebug() << "Condition:" << lastCondition << "Separator: None";
    //通过and和or分离
    QStringList conditions = text.split(regex_divide, QString::SkipEmptyParts);


    qDebug() << "Separated Conditions:";
    for (const QString &cond : conditions)
    {
        QRegularExpression regex_opperand("(\\w+)\\s*((.*)+)", QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = regex_opperand.match(cond.trimmed());
        QString columnname = match.captured(1);

        names_array.push_back(columnname.toStdString());
        QString singlecolumn = match.captured(2);
        cons_string_array.push_back(singlecolumn_constraints(singlecolumn));

        qDebug() << columnname;
        //  qDebug() << QString::fromStdString(singlecolumn_constraints(singlecolumn));




        qDebug() <<singlecolumn;
    }
    // qDebug() <<QString::fromStdString(res);
    cons_string_array.push_back(expression_res);
    pair.first = names_array;
    pair.second = names_array;
    return pair;
}
bool Parser::login(QString  name,QString password)
{
    authorization au;
    if(au.user_login(name.toStdString(),password.toStdString()))
    {
        user = name.toStdString();
        return true;
    }
     return false;
}
 bool Parser::register_user(QString  name,QString password)
 {
     authorization au;
     if(au.user_register(name.toStdString(),password.toStdString()))
     {
      return true;
     }
      return false;
 }
 bool Parser::grant(QString text)
 {
     QRegularExpression regex_grant("\\bgrant\\s+(\\w+)\\s+on\\s+(\\w+)\\s+to\\s+(\\w+)\\s*;", QRegularExpression::CaseInsensitiveOption);

     QRegularExpressionMatch match_grant = regex_grant.match(text);

     if (match_grant.hasMatch())
     {

        QString permission = match_grant.captured(1);
        QString database = match_grant.captured(2);
        QString user = match_grant.captured(3);

        authorization au;
        au.grantpermission(permission.toStdString(),database.toStdString(),user.toStdString());

         return true;


     }
     return false;
 }

