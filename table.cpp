
﻿#include "table.h"
static const string DBadd="D:\\testsssbin\\";
Table::Table(const string& tableName):tableName(tableName)
{
    add=DBadd+tableName;
    //未找到该表
    if(!readFromFile(tableName)) cerr<<"Error finding tabel."<<endl;
    line_num=lines.size();
    row_num=0;
    for (const auto& str : rows)
        row_num++;
}

Table::Table(const vector<tableRows>& newTable,const string& tableName):tableName(tableName)
{
    CreateTable(newTable,tableName);
    add=DBadd+tableName;
    line_num=0;
    rows=newTable;
    row_num=0;
    for (const auto& str : rows)
        row_num++;
}

Table::~Table()
{

}

void Table::test()
{
    for (const auto& str : lines)
        cout<<str<<endl;
    cout<<line_num<<endl;
    for (const auto& a : rows)
        cout<<a.rowName<<" "<< a.rowType<<" "<< a.typeLen<<" "<< a.rowConstrain<<" "<<endl;
    cout<<row_num<<endl;
//    rows.push_back({"asduasd",DOUBLE,15,NONE});
//    rows.push_back({"assadsd",DOUBLE,15,NONE});
//    rows.push_back({"akhlasd",DOUBLE,24,NONE});
//    rows.push_back({"asduaiytthjyt",INT,15,NONE});
//    saveToFile(save_mode::BOTH);


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
    //先解析是否有对应的列要求
    if(correspond!="")
    {

    }
    //检查属性要求是否合格

    //插入lines（内存暂存表结构）
    lines.push_back(content);
    line_num++;
    //是否需要在此处保存？
    saveToFile();
}

bool Table::CreateTable(const vector<tableRows>& newTable,const string& tableName)
{
    QDir dir;
    //要改成常量
    QString tableDirPath = QString::fromStdString(DBadd+tableName);
    string tableDirPath1=DBadd+tableName;
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

    //处理newTable结构体数据
    vector<string> attribute;//属性值
    cout<<"done"<<endl;
    for (const auto& str : newTable)
    {
        attribute.push_back(str.rowName);
        //
        //
        //
        //
        //
        //
        //
        //
        //
        //
    }

    //创建属性文件
    ofstream file_A(tableDirPath1+"\\attribute.bin",ios::binary);
    //ofstream file_A("aaaaaaa.bin",ios::binary);
    if (file_A.is_open())
    {
        cout<<"createFile"+tableDirPath1+"\\attribute.bin"<<endl;
        for (const auto& str : attribute)
        {
            //cout<<str.c_str()<<endl;
            file_A.write(str.c_str(), str.size());
            file_A.write("|",1); // 写入分隔符
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
    //写入属性
//    if(mode==save_mode::BOTH||mode==save_mode::ATTRIBUTE)
//    {
//        ofstream file_A(add+"\\attribute.bin",ios::binary);
//        if (!file_A.is_open())
//        {
//            cerr << "Error opening file " <<endl;
//            return false;
//        }
//        for (const auto& s : rows)//直接写入结构体
//        {
//            file_A.write(reinterpret_cast<const char*>(&s), sizeof(struct tableRows));
//        }
//        file_A.close();
//    }
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
                    << s.typeLen << "|"
                       //
                       //
                       //
                       //
                       //等等等等
                    << s.rowConstrain << "\n";
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
    QString tableDirPath = QString::fromStdString(DBadd+tableName);
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
//    rows.clear();
//    ifstream file_A(add+"\\attribute.bin",ios::binary);
//    if (file_A.is_open())
//    {
//        tableRows tempStruct;
//        while (file_A.read(reinterpret_cast<char*>(&tempStruct), sizeof(tableRows)))
//        {
//            cout<<tempStruct.rowName<<endl;
//            //rows.push_back(tempStruct);
//        }
//        file_A.close();
//    }
//    else
//    {
//        cerr << "Error opening file: " << add+"\\content.bin" << endl;
//        return false;
//    }
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
            getline(ss, item, '|');
            row.rowName = item;
            getline(ss, item, '|');
            row.rowType = static_cast<type>(stoi(item));
            getline(ss, item, '|');
            row.typeLen = stoi(item);
            getline(ss, item, '|');
            row.rowConstrain = static_cast<constrain>(stoi(item));
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

