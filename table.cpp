#include "table.h"
#include <QDebug>
#include <cstdio>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QDir>
//kenny
Table::Table(const string& tableName,DB * db):tableName(tableName),db(db)
{
    string dbadd = db->CurrentDbPath.toStdString();
    std::cout<<dbadd<<endl;
    add=dbadd+"\\"+tableName;
    //std::cout<<add;
    //未找到该表
    if(!readFromFile(tableName)) cerr<<"Error finding tabel."<<endl;
    line_num=lines.size();
    row_num=0;
    for (const auto& str : rows)
        row_num++;
}

//kenny
Table::Table(const vector<tableRows>& newTable,const string& tableName,DB * db):tableName(tableName),rows(newTable),db(db)
{
    string dbadd = db->CurrentDbPath.toStdString();
    add=dbadd+"\\"+tableName;
    CreateTable(rows,tableName);
    line_num=0;
    //rows=newTable;
    row_num=0;
    for (const auto& str : rows)
        row_num++;
}
//kenny

Table::~Table()
{

}

void Table::test()
{
cout<<add;
getContrain("stduent");
for(auto ppp:lines)cout<<ppp<<endl;

}

bool Table::dropTable()
{
    QString add_Q=QString::fromStdString(add);
    QDir dir(add_Q);
    if (!dir.exists())
    {
        qDebug() << "Folder does not exist:" << add_Q;
        return false;
    }

    if (dir.removeRecursively())
    {
        qDebug() << "Folder deleted successfully:" << add_Q;
        return true;
    }
    else
    {
        qDebug() << "Error deleting folder:" << add_Q;
        return false;
    }
}

bool Table::instertTOTable(const string& content, const string& correspond)
{
    this_row.clear();
    this_row=rows;
    string newline="";
    vector<string> correspond_v=splitByPipe(correspond);//列名
    vector<string>  content_v=splitByPipe(content);//每列对应内容
    //content先全赋值为默认值
    for(auto thing: this_row)
    {
        thing.content=thing.default_content;
    }
    //先解析是否有对应的列要求
    if(correspond!="")//有要求插入（）
    {
        if(content_v.size()!=rows.size()||correspond_v.size()!=rows.size())//数目不一直接报错
        {
            cerr<<"no matching insert error"<<endl;
            return false;
        }
        else
        {
            for(unsigned j =0;j<correspond_v.size();++j)//对每个列名进行检查，不存在相关列名直接报错,将其插入当前列
            {
                unsigned int i=0;
                bool flag=true;
                while (i<rows.size()&&flag)
                {
                    rows[i].rowName==correspond_v[j]?flag=false:++i;
                }
                if(i>=rows.size())
                {
                    cerr<<"no such row!error"<<endl;
                    return false;
                }
                else
                {
                    //检查限制

                    if(!checkType(this_row[i].rowType,content_v[j]))//检查类型错误
                    {
                        cerr<<"type wrong!error"<<endl;
                        return false;
                    }
                    if(this_row[i].constrainMessage==true&&!checkConstrains(getContrain(this_row[i].rowName),this_row[i].rowType,content_v[j]))
                    {
                        cerr<<"cannot insert for constrain limitations!"<<endl;
                        return false;
                    }
                    this_row[i].content=content_v[j];

                }
            }
        }
    }
    else //无要求插入，默认插入
    {
        if(content_v.size()!=rows.size())//数目不一直接报错
        {
            cerr<<" no matching insert error"<<endl;
            return false;
        }
        else
        {
            for(unsigned int i=0;i<(unsigned int)row_num;++i)
            {
                if(!checkType(this_row[i].rowType,content_v[i]))//检查类型错误
                {
                    cerr<<"type wrong!error"<<endl;
                    return false;
                }
                //检查限制
                if(this_row[i].constrainMessage==true&&!checkConstrains(getContrain(this_row[i].rowName),this_row[i].rowType,content_v[i]))
                {
                    cerr<<"cannot insert for constrain limitations!"<<endl;
                    return false;
                }
                this_row[i].content=content_v[i];
            }
        }
    }
    //将当前列数据写成newline插入lines
    for(unsigned int i=0;i<this_row.size();++i)
    {
        newline.append(this_row[i].content);
        if(i<this_row.size()-1)//最后一个属性后不打|
         newline.append("|");
    }
    //插入lines（内存暂存表结构）
    cout<<"newline"<<newline<<" linenum"<<lines.size();
    lines.push_back(newline);
    line_num++;
    //是否需要在此处保存？
    saveToFile();
    return true;
}
bool Table::deleteFromTable(const delete_mode& mode,const vector<string>&rowname,const vector<string>&constrainMessage,const string& operation)
{

    if(mode==delete_mode::ALL)//清空整个表
    {
        ofstream file_C(add+"\\content.bin",ios::binary);
        if (!file_C.is_open())
        {
            cerr << "Error opening file " <<endl;
            return false;
        }
        else
        {
            lines.clear();
            file_C.write("",0);
            cout<<"delete successfully!"<<endl;
        }

        file_C.close();
        return true;
    }
    else if (mode==delete_mode::SELECT)//带限制的删除
    {
        //储存删除后的新表
        vector<string>new_lines;
        //先检查列限制数目是否匹配
        if(rowname.size()!=constrainMessage.size())
        {
            cerr<<"num in delete !not match !"<<endl;
            return false;
        }
        //获得限制所在的列，便于操作
        vector<unsigned int> aim_row;//目标列所在编号
        for(unsigned int i=0;i<rowname.size();++i)
        {
            for(unsigned int j=0;j<(unsigned)row_num;++j)
            {
                if(rowname[i]==rows[j].rowName)
                    aim_row.push_back(j);
            }
        }
        if(rowname.size()!=aim_row.size())
        {
            cerr<<"now such row !"<<endl;
            return false;
        }
        //逐行检测,i为行号
        for(unsigned int i=0;i<(unsigned int)line_num;++i)
        {
            checkLines(lines[i]);
            bool flag;
            vector<bool>checks;
            for(unsigned int j=0;j<aim_row.size();++j)//每一个限制检查
            {
               bool f=checkConstrains(constrainMessage[j],this_row[aim_row[j]].rowType,this_row[aim_row[j]].content);
               checks.push_back(f);
            }
            //flag=eva
            flag=evaluate(operation,checks);
            //检查是否符合所有约束,不符合就保留
           if(!flag) new_lines.push_back(lines[i]);
        }
        lines=new_lines;
        //for(auto ppp:lines)cout<<ppp<<endl;
        //保存文件
        saveToFile();
        return true;
    }
    else return false;

}

//实在不想写了，这里只能修改一列，sql修改多列就多次调用
bool Table::alterTable(const Table::alter_mode &mode, const Table::alter_class &class_A, const string &content,const string& constrainn)
{
    if(mode==alter_mode::DROP&&class_A==alter_class::ROW)//删除列(f)
    {
        //检查是否有这列，序号为i的列是要删除的
        unsigned int i=0;
        for(;i<(unsigned int)row_num;++i)
        {
            if(rows[i].rowName==content)
                break;
        }
        if(i>=(unsigned int )row_num)
        {
            cerr<<"no such row!!"<<endl;
            return false;
        }
        //主键禁止删除
        if(rows[i].isPrimaryKey)
        {
            cerr<<"cannot drop primarykey!"<<endl;
            return false;
        }
        //如果有限制要删除限制文件
        if(rows[i].constrainMessage)
        {
            alterTable(alter_mode::DROP,alter_class::CONSTRAIN,rows[i].rowName);
        }
        //删除该列 vec.erase(vec.begin() + index);
        //先删除每行中这列的数据
        vector<string>newlines;
        for(auto l:lines)
        {
            string newline;
            checkLines(l);
            for(unsigned int t=0;t<this_row.size();++t)
            {
                if(t==i) continue;
                newline.append(this_row[t].content);
                newline.append("|");
            }
            if (!newline.empty() && newline.back() == '|')
            {
                // 删除最后一个字符
                newline.pop_back();
            }
            newlines.push_back(newline);
        }
        lines=newlines;
        //删除列数据
        rows.erase(rows.begin() + (int)i);
        saveToFile(save_mode::BOTH);
    }
    else if(mode==alter_mode::DROP&&class_A==alter_class::PRIMARY_KRY)//删除主键(f)
    {
        vector<string> rownames=splitByPipe(content);
        for(auto name:rownames)
        {
            unsigned int i=0;
            for(;i<(unsigned int)row_num;++i)
            {
                if(rows[i].rowName==name&&rows[i].isPrimaryKey==true)
                    rows[i].isPrimaryKey=false;
                else if (rows[i].isPrimaryKey==false)
                {
                    cerr<<rows[i].rowName<<"is not primary,change stop here!"<<endl;
                    return false;
                }
            }
            if (i>=(unsigned int)row_num)
            {
                cerr<<"no such row,change stop here!"<<endl;
                return false;
            }
            saveToFile(save_mode::ATTRIBUTE);
        }
    }
    else if(mode==alter_mode::DROP&&class_A==alter_class::CONSTRAIN)//删除约束(f)
    {
        //检查是否有这列
        unsigned int i=0;
        for(;i<(unsigned int)row_num;++i)
        {
            if(rows[i].rowName==content)
                break;
        }
        if(i>=(unsigned int )row_num)
        {
            cerr<<"no such row!!"<<endl;
            return false;
        }
        if(rows[i].constrainMessage==false)
        {
            cerr<<"no constrains to drop!"<<endl;
        }
        else
        {
            rows[i].constrainMessage=false;
            saveToFile(save_mode::ATTRIBUTE);
            //删除文件constrain
            // 将 std::string 转换为 const char*
            string file=add+"\\"+rows[i].rowName+"constrains.bin";
            const char* cFilePath = file.c_str();

            // 使用 C 标准库的 remove 函数删除文件
            int result = std::remove(cFilePath);

            // 检查删除操作是否成功
            if (result == 0)
            {
                return true; // 文件成功删除
            }
            else
            {
                cerr<<"cannot remove the constrain file!"<<endl;
                return false; // 删除失败
            }
        }
    }
    else if(mode==alter_mode::ADD&&class_A==alter_class::ROW)//增加列   //只允许传 列名|类型
    {
        vector<string> con=splitByPipe(content);
        //检查是否有这列
        for(unsigned int i=0;i<(unsigned int)row_num;++i)
        {
            if(rows[i].rowName==content)
            {
                cerr<<"had such row!!canno add row"<<endl;
                return false;
            }
        }
        type tp;
        if(con[1]=="INT")
        {
            tp=type::INT;
        }
        if(con[1]=="DOUBLE")
        {
            tp=type::DOUBLE;
        }
        if(con[1]=="CHAR")
        {
            tp=type::CHAR;
        }
        rows.push_back(tableRows(con[0],tp));
        //为每一行增加该列
        for(unsigned int i=0;i<lines.size();++i)
        {
            lines[i]=lines[i]+"|"+"";
        }
        saveToFile(save_mode::BOTH);
    }
    else if(mode==alter_mode::ADD&&class_A==alter_class::CONSTRAIN)//增加约束(uf)  只允许增加一个约束 此处用到最后一个约束 da|666
    {
        //检查是否有这列

        for(unsigned int i=0;i<(unsigned int)row_num;++i)
        {
            if(rows[i].rowName==content)
            {
                cerr<<"already have such row!!"<<endl;
                return false;
            }
        }

        //重写约束问文件
        //cerr<<message<<endl;
        string newCons=getContrain(content);
        newCons.append(constrainn);
        newCons[0] = 'h';
        setContrain(content,newCons);
    }
    else if(mode==alter_mode::RENAME&&class_A==alter_class::ROW)//列重命名,传两个string|string，一个是旧名，一个是新名(f)
    {
        vector<string> con=splitByPipe(content);
        //检查是否有这列
        unsigned int i=0;
        for(;i<(unsigned int)row_num;++i)
        {
            if(rows[i].rowName==con[0])
                break;
        }
        if(i>=(unsigned int )row_num)
        {
            cerr<<"no such row!!"<<endl;
            return false;
        }
        rows[i].rowName=con[1];
        saveToFile(save_mode::ATTRIBUTE);
        if(rows[i].constrainMessage)
        {
            //修改constrain文件名
            // 将 std::string 转换为 QString
            QString oldFileQString = QString::fromStdString(add+"\\"+con[0]+"constrains.bin");
            QString newFileQString = QFileInfo(oldFileQString).path() + "/" + QString::fromStdString(con[1]+"constrains.bin");

            // 使用 QFile 进行重命名
            QFile file(oldFileQString);
        }
    }
    else if(mode==alter_mode::RENAME&&class_A==alter_class::TABLE)//表重命名，传1个string，新名(f)
    {
        //重命名文件夹
        // 将 std::string 转换为 QString
        QString oldDirQString = QString::fromStdString(add);
        qDebug() <<  oldDirQString<<888;
        QString newDirQString = QDir(oldDirQString).filePath(QString::fromStdString(add));
        qDebug() <<  newDirQString<<999;

        // 使用 QDir 进行重命名
        QDir dir(oldDirQString);
        if (!dir.exists())
        {
            qDebug() << "Old table name does not exist:" << oldDirQString;
            return false;
        }

        // 注意：QDir 没有直接的 rename 方法，但我们可以使用 QFile 来重命名目录
        // 因为 QFile::rename() 可以用于文件和目录
        QFile file(oldDirQString);
        if(file.rename(newDirQString))cout<<"yess"<<endl;else cout<<"no"<<endl;
    }
    else if(mode==alter_mode::MODIFY&&class_A==alter_class::ROW)//(f)修改列的字段类型！！！必须列为空才能修改，有约束会删除约束和默认值  格式  name|INT
    {
        vector<string> contents=splitByPipe(content);
        //获得该列的编号row_num
        unsigned int rownum;
        unsigned int i=0;
        for(;i<rows.size();++i)
        {
            if(rows[i].rowName==contents[0])
            {
                rownum=i;
                break;
            }
        }
        //未找到该列
        if(i>=rows.size())
        {
            cerr<<"no such row!!"<<endl;
            return false;
        }
        //检查是否该列数据都为空
        bool flag=true;
        for(auto c:lines)
        {
            checkLines(c);
            flag*=(this_row[rownum].content=="");
        }
        //有不空的
        if(!flag)
        {
            cerr<<"some data is not null,modify fobidden!"<<endl;
        }
        //该代码块进行修改工作
        {
            if(contents[1]=="INT")
            {
                rows[rownum].rowType=type::INT;
            }
            if(contents[1]=="DOUBLE")
            {
                rows[rownum].rowType=type::DOUBLE;
            }
            if(contents[1]=="CHAR")
            {
                rows[rownum].rowType=type::CHAR;
            }
            //删除默认值
            rows[rownum].default_content="";
            saveToFile(save_mode::ATTRIBUTE);
        }
        //删除约束
        alterTable(alter_mode::DROP,alter_class::CONSTRAIN,rows[rownum].rowName);

    }
    else if(mode==alter_mode::MODIFY&&class_A==alter_class::ROW_DEFAULT)//(f)修改列的字段默认值！！！     格式  name|默认值
    {
        vector<string> contents=splitByPipe(content);
        //获得该列的编号row_num
        unsigned int rownum;
        unsigned int i=0;
        for(;i<rows.size();++i)
        {
            if(rows[i].rowName==contents[0])
            {
                rownum=i;
                break;
            }
        }
        //未找到该列
        if(i>=rows.size())
        {
            cerr<<"no such row!!"<<endl;
            return false;
        }

        //检查新默认值是否合规
        if(!checkType(rows[rownum].rowType,contents[1]))
        {
            cerr<<"wrong default type!"<<endl;
            return false;
        }
        rows[rownum].default_content=contents[1];
        saveToFile(save_mode::BOTH);
    }
    else
    {
        cerr<<"wrong mode!!"<<endl;
        return false;
    }
    return true;
}

bool Table::updateTable(const vector<string>&rowname,const vector<string>&goal,const vector<string>&cname,const vector<string>&constrainMessage,const string& operation)
{
    //传入数据不符合规范，直接报错
    if(rowname.size()!=goal.size())
    {
        cerr<<"wrong data!"<<endl;
        readFromFile(tableName);//防止lines存在已经修改数据
        return false;
    }
    //先检查列限制数目是否匹配
    if(cname.size()!=constrainMessage.size())
    {
        cerr<<"num in delete !not match !"<<endl;
        return false;
    }
    //获得限制所在的列编号
    vector<unsigned int> c_row;//限制列所在编号
    for(unsigned int i=0;i<cname.size();++i)
    {
        for(unsigned int j=0;j<(unsigned)row_num;++j)
        {
            if(cname[i]==rows[j].rowName)
                c_row.push_back(j);
        }
    }
    if(cname.size()!=c_row.size())
    {
        cerr<<"now such row !"<<endl;
        return false;
    }
    //获得目标列所在编号
    unsigned int size=rowname.size();
    vector<unsigned int> aim_row;//目标列所在编号
    //储存更新后的新表
    vector<string>new_lines;
    for(unsigned int i=0;i<size;++i)//对每个项目（set）进行检查
    {
        //先查找是否有这样的列checkType(rows[j].rowType,goal[i])
        unsigned int j=0;

        for(;j<(unsigned int)row_num;++j)
        {
            if(rows[j].rowName==rowname[i])//找到了这样的列
            {
                aim_row.push_back(j);
                if(checkType(rows[j].rowType,goal[i]))//检查update的目标类型是否合规
                {
                    //检查更新的数据是否合规||限制
                    if(rows[j].constrainMessage==true&&checkConstrains(getContrain(rows[j].rowName),rows[j].rowType,goal[i]))
                    break;
                    else if(rows[j].constrainMessage==false) break;
                    else
                    {
                        cerr<<"wrong goal type"<<endl;
                        return false;
                    }
                }
                else
                {
                    cerr<<"wrong type!"<<endl;
                    readFromFile(tableName);//防止lines存在已经修改数据
                    return false;
                }
            }
        }
        if(j>=(unsigned int)row_num)//没有找到这样的列
        {
            cerr<<"no such row!"<<endl;
            readFromFile(tableName);//防止lines存在已经修改数据
            return false;
        }
    }
     //提取的列数目不足
    if(aim_row.size()!=rowname.size())
    {
        cerr<<"wrong row name num!"<<endl;
    }

    //再查找where限制的行，删除这个行并将content更新到新行上
    //注意，每个return false前加一个readFromFile(tableName);
    //逐行检测,i为行号
    for(unsigned int i=0;i<lines.size();++i)
    {
        checkLines(lines[i]);
        bool flag=true;
        vector<bool>checks;
        for(unsigned int j=0;j<c_row.size();++j)//每一个限制检查
        {
           bool f=checkConstrains(constrainMessage[j],this_row[c_row[j]].rowType,this_row[c_row[j]].content);
           cout<<constrainMessage[j]<<"   "<<this_row[c_row[j]].content<<"shi"<<f<<endl;
           checks.push_back(f);
        }
        flag=evaluate(operation,checks);
        cout<<"flag"<<flag<<endl;
        //检查是否符合所有约束,不符合就保留，符合就更改
//        for(auto poi:this_row)
//            cout<<poi.content<<endl;
       if(flag) //符合就更改
       {
           for(unsigned int t=0;t<aim_row.size();++t)
           {
               this_row[aim_row[t]].content=goal[t];
           }
//           for(auto poi:this_row)
//               cout<<poi.content<<endl;
           string newline;
           for(unsigned int t=0;t<this_row.size();++t)
           {
               //cout<<9859559<<this_row[t].content<<endl;
               newline.append(this_row[t].content);
               if(t<this_row.size()-1)//最后一个属性后不打|
                newline.append("|");
           }
           new_lines.push_back(newline);
       }
       else//不符合就保留
       {
//           cout<<777<<endl;
            string newline=lines[i];
           new_lines.push_back(newline);
       }
    }
//    for(auto poi:new_lines)
//        cout<<poi<<endl;
//    cout<<new_lines.size()<<endl;
    lines=new_lines;
    //保存
    saveToFile();
    return true;
}

void Table::checkLines(const string& in)//获得一列到类中，进行操作
{
    this_row.clear();
    vector<string> contents=splitByPipe(in);
    this_row=rows;
    for(unsigned int i=0;i<rows.size();++i)
    {
        this_row[i].content=contents[i];
    }
}

string Table::getContrain(const string& rowname)
{
    string fileName=add+"\\"+rowname+"constrains.bin";
    cout<<fileName<<endl;
    QFile file(QString::fromStdString(fileName));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // 处理打开文件失败的情况
            qWarning() << "Could not open file:" << fileName.c_str();
            return std::string(); // 返回一个空字符串或抛出异常
        }

        QTextStream in(&file);
        QString content = in.readAll();

        file.close();
        return content.toStdString(); // 将QString转换为std::string
}

bool Table::setContrain(const string &rowname,const string& contents)
{
    // 将std::string转换为QString
        string filePath=add+"\\"+rowname+"constrains.bin";
        QString filePathQString = QString::fromStdString(filePath);
        QString contentQString = QString::fromStdString(contents);

        // 创建QFile对象
        QFile file(filePathQString);

        // 打开文件以写入，如果文件不存在则创建它
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 处理打开文件失败的情况
            qWarning() << "Could not open file for writing:" << filePathQString;
            return false;
        }

        // 创建QTextStream对象以写入文件
        QTextStream out(&file);

        // 写入内容
        out << contentQString;

        // 关闭文件
        file.close();

        return true;
}




bool Table::CreateTable(const vector<tableRows>& newTable,const string& tableName)
{
    QDir dir;
    //要改成常量
    QString tableDirPath = QString::fromStdString(add);
    string tableDirPath1=add;
    //创建表文件夹,文件夹内存放表相关信息文件
    if (!dir.exists(tableDirPath))
    {
          // 如果不存在，创建文件夹
          if (dir.mkpath(tableDirPath))
          {
              qDebug() << "Folder created successfully at" << tableDirPath;
          }
          else
          {
              qDebug() << "Error creating folder.";
              return false;
          }
      }
    else
    {
          qDebug() << "Folder already exists at" << tableDirPath;
           return false;
    }
    //创建属性文件
    ofstream file_A(tableDirPath1+"\\attribute.bin",ios::binary);
    //ofstream file_A("aaaaaaa.bin",ios::binary);
    if (file_A.is_open())
    {
        cout<<"createFile"+tableDirPath1+"\\attribute.bin"<<endl;
        for (const auto& s: newTable)
        {
            file_A << s.rowName << "|"
                   << s.rowType << "|"
                   << s.isPrimaryKey << "|"
                  << s.forignKeyName << "|"
                << s.forignKeyTable << "|"
                   << s.default_content<< "|"
                      << s.constrainMessage<< "|"
                         <<  "\n";
        }
        file_A.close();
    }
    else
    {
        cerr << "Error opening file " <<tableDirPath1+"\\attribute"<<endl;
        return false;
    }
    //创建表文件
    ofstream file_C(tableDirPath1+"\\content.bin",ios::binary);
    if (file_C.is_open())
    {
        cout<<"创建文件"+tableDirPath1+"\\content.bin"<<endl;
        file_C.close();
    }
    else
    {
        cerr << "Error opening file " <<endl;
        return false;
    }
    return true;
}

bool Table::saveToFile(save_mode mode)
{
    //写入表中数据
    if(mode==save_mode::BOTH||mode==save_mode::CONTENT)
    {
        ofstream file_C(add+"\\content.bin",ios::binary);
        if (!file_C.is_open())
        {
            cerr << "Error opening file " <<endl;
            return false;
        }
        for (const auto& str : lines)
        {
            file_C.write(str.c_str(), str.size());
            file_C.write("\n",1); // 写入分行符
        }
        file_C.close();
    }
    if(mode==save_mode::BOTH||mode==save_mode::ATTRIBUTE)
    {
        ofstream file_A(add+"\\attribute.bin",ios::binary);
        if (!file_A.is_open())
        {
            cerr << "Error opening file " <<endl;
            return false;
        }
        for (const auto& s : rows)//直接写入结构体
        {
            file_A << s.rowName << "|"
                   << s.rowType << "|"
                   << s.isPrimaryKey << "|"
                  << s.forignKeyName << "|"
                << s.forignKeyTable << "|"
                   << s.default_content<< "|"
                      << s.constrainMessage<< "|"
                         <<  "\n";
        }
        file_A.close();
    }
    return true;
}

bool Table::readFromFile(const string& tableName)
{
    const char delimiter = '\n';
    QDir dir;
    //要改成常量
    QString tableDirPath = QString::fromStdString(add);
    //表不存在返回false
    if (!dir.exists(tableDirPath))
    {
        return false;
    }
    //创建文件输入流
    //1.读取表中数据
    lines.clear();
    ifstream file_C(add+"\\content.bin",ios::binary);
    if (file_C.is_open())
    {
        string str;
        while (std::getline(file_C, str, delimiter))
        { // 以分隔符为界读取字符串
            lines.push_back(str);
        }
        file_C.close();
    }
    else
    {
        cerr << "Error opening file: " << add+"\\content.bin" << endl;
        return false;
    }
    //2.读取表中属性
    rows.clear();
    ifstream file_A(add+"\\attribute.bin",ios::binary);
    if (file_A.is_open())
    {
        string line;
        while (getline(file_A, line))
        {
            tableRows row;
            //字符流
            stringstream ss(line);
            string item;
            //rowName
            getline(ss, item, '|');
            row.rowName = item;
            //rowType
            getline(ss, item, '|');
            row.rowType = static_cast<type>(stoi(item));
            //isPrimaryKey
            getline(ss, item, '|');
            row.isPrimaryKey = static_cast<bool>(stoi(item));
            //forignKeyName
            getline(ss, item, '|');
            row.forignKeyName = item;
            //forignKeyTable
            getline(ss, item, '|');
            row.forignKeyTable = item;
            //default_content
            getline(ss, item, '|');
            row.default_content = item;
            //constrainMessage
            getline(ss, item, '|');
            row.constrainMessage = static_cast<bool>(stoi(item));
            //row.rowConstrain = static_cast<constrain>(stoi(item));

            rows.push_back(row);
        }
        file_A.close();
        cout << "Data read from file successfully." << endl;
    }
    else
    {
        cerr << "Unable to open file: "  << endl;
    }
    return true;
}
// 打印函数
void Table::show() {
    // 打印表头
    for (const auto& row : rows) {
        std::cout << row.rowName << "\t";
    }
    std::cout << std::endl;

    // 打印分隔线
    for (size_t i = 0; i < rows.size(); ++i) {
        std::cout << "----\t";
    }
    std::cout << std::endl;

    // 打印每一行数据
    for (const auto& line : lines) {
        std::istringstream iss(line);
        std::string value;
        while (std::getline(iss, value, '|')) {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
}
//kenny
bool Table::selectallfrom(const string& tableName,const vector<string>rowname,const vector<string>&constrainMessage,const string& operation)
{
    //存选中列对应的在rows的下标位置
    vector<int> accordrowsindex;
    //将查询到的数据创建个表，用于建表
    vector<tableRows> newtablerows;
    //用于插入数据
    string newtablerowsname;

        for(unsigned int j = 0;j<rows.size();j++)
        {

            //存的是下标
            accordrowsindex.push_back(j);
            //存的是要新建表的列
            newtablerowsname.append(rows[j].rowName);
            newtablerows.push_back(rows[j]);


        }

//    //打印列名
//    for(const auto& selectedrow:selectedrows)
//    {
//        cout<<selectedrow<<" ";
//    }
//    cout<<endl;

    //建表
    string newtablename = tableName+"select";
    Table newtable(newtablerows,newtablename,this->db);



    //wvan2233
    //获得所需的行
    vector<string> aim_line;//目标列所在编号
    //先检查列限制数目是否匹配
    if(rowname.size()!=constrainMessage.size())
    {
        cerr<<"num in delete !not match !"<<endl;
        return false;
    }
    //获得限制所在的列，便于操作
    vector<unsigned int> aim_row;//目标列所在编号
    for(unsigned int i=0;i<rowname.size();++i)
    {
        for(unsigned int j=0;j<(unsigned)row_num;++j)
        {
            if(rowname[i]==rows[j].rowName)
                aim_row.push_back(j);
        }
    }
    if(rowname.size()!=aim_row.size())
    {
        cerr<<"now such row !"<<endl;
        return false;
    }
    //逐行检测,i为行号
    for(unsigned int i=0;i<(unsigned int)line_num;++i)
    {
        checkLines(lines[i]);
        bool flag;
        vector<bool>checks;
        for(unsigned int j=0;j<aim_row.size();++j)//每一个限制检查
        {
           bool f=checkConstrains(constrainMessage[j],this_row[aim_row[j]].rowType,this_row[aim_row[j]].content);
           checks.push_back(f);
        }
        flag=evaluate(operation,checks);
        //检查是否符合所有约束,符合就保留
       if(flag) aim_line.push_back(lines[i]);
    }


    //kenny
    //打印每一行
    for(const auto & line:aim_line)
    {
        // 使用std::stringstream将输入字符串通过|分割为子字符串
           std::stringstream ssline(line);
           std::string token;
           std::vector<std::string> tokens;
           while (std::getline(ssline, token, '|')) {
               tokens.push_back(token);
           }
           //
           string newdata;
           //输出每一行第i列的数据
        for(unsigned int i=0;i<accordrowsindex.size();i++)
        {
            cout<<tokens[i];
            //加|成新的一行数据
            newdata.append(tokens[i]);
             newdata.append("|");
        }
        cout<<endl;
        //去掉最后一个|,建表
        newdata.pop_back();
        newtable.instertTOTable(newdata,newtablerowsname);
    }
    return true;
}
//kenny
bool Table::selectfrom(const vector<string>& selectedrows ,const string& tableName,const vector<string>rowname,const vector<string>&constrainMessage,const string& operation)
{
    //存选中列对应的在rows的下标位置
    vector<int> accordrowsindex;
    //将查询到的数据创建个表，用于建表
    vector<tableRows> newtablerows;
    //用于插入数据
    string newtablerowsname;
    //若输入不存在的列名，则报错
    for(unsigned int i=0;i<selectedrows.size();i++)//int 有警告
    {
        for(unsigned int j = 0;j<rows.size();j++)
        {
        if(selectedrows[i] == rows[j].rowName)
        {
            //存的是下标
            accordrowsindex.push_back(j);
            //存的是要新建表的列
            newtablerowsname.append(selectedrows[i]);
            newtablerows.push_back(rows[j]);

            break;
        }
        else
        {
            if(j == rows.size()-1)
            {
                cerr << "unexist such column "  << endl;
                return false;
            }
            else
            {
             continue;
            }
        }
        }
    }

    //打印列名
    for(const auto& selectedrow:selectedrows)
    {
        cout<<selectedrow<<" ";
    }
    cout<<endl;

    //建表
    string newtablename = tableName+"select";
    Table newtable(newtablerows,newtablename,this->db);


    //wvan2233
    //获得所需的行
    vector<string> aim_line;//目标列所在编号
    //先检查列限制数目是否匹配
    if(rowname.size()!=constrainMessage.size())
    {
        cerr<<"num in delete !not match !"<<endl;
        return false;
    }
    //获得限制所在的列，便于操作
    vector<unsigned int> aim_row;//目标列所在编号
    for(unsigned int i=0;i<rowname.size();++i)
    {
        for(unsigned int j=0;j<(unsigned)row_num;++j)
        {
            if(rowname[i]==rows[j].rowName)
                aim_row.push_back(j);
        }
    }
    if(rowname.size()!=aim_row.size())
    {
        cerr<<"now such row !"<<endl;
        return false;
    }
    //逐行检测,i为行号
    for(unsigned int i=0;i<(unsigned int)line_num;++i)
    {
        checkLines(lines[i]);
        bool flag;
        vector<bool>checks;
        for(unsigned int j=0;j<aim_row.size();++j)//每一个限制检查
        {
           bool f=checkConstrains(constrainMessage[j],this_row[aim_row[j]].rowType,this_row[aim_row[j]].content);
           checks.push_back(f);
        }
        flag=evaluate(operation,checks);
        //检查是否符合所有约束,符合就保留
       if(flag) aim_line.push_back(lines[i]);
    }


    //kenny
    //打印每一行
    for(const auto & line:aim_line)
    {
        // 使用std::stringstream将输入字符串通过|分割为子字符串
           std::stringstream ssline(line);
           std::string token;
           std::vector<std::string> tokens;
           while (std::getline(ssline, token, '|'))
           {
               tokens.push_back(token);
           }
           //
           string newdata;
           //输出每一行第i列的数据
        for(unsigned int i=0;i<accordrowsindex.size();i++)
        {
            cout<<tokens[i];
            //加|成新的一行数据
            newdata.append(tokens[i]);
             newdata.append("|");
        }
        cout<<endl;
        //去掉最后一个|,建表
        newdata.pop_back();
        newtable.instertTOTable(newdata,newtablerowsname);
    }
    return true;
}
