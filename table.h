
﻿#ifndef TABLE_H
#define TABLE_H
#include <QFile>
#include <QDir>
#include <QDebug>
#include<string>
#include<iostream>
#include <QDir>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;



enum type
{
    INT,DOUBLE,VARCHAR,CHAR
};

enum constrain{NONE=0};

struct tableRows//要给一个静态变量int
{
    string rowName;//列名
    type rowType;//列的数据要求
    int typeLen;//长度要求
    constrain rowConstrain;//列的限制
    //bool isPrimaryKey;//是否主键
    //string forignKeyName;//外键名称//多个外键怎么办？？？？？？？？？？？？？
    //string forignKeyTable;//外键所属表名称
};//简历表信息的结构体


class Table
{
    static const char delimiter = '\n';//文件写入分隔符
    enum class save_mode{CONTENT=-1,ATTRIBUTE=1,BOTH=0};//保存文件的选项
private:
    string add;//文件夹保存地址
    string tableName;//表名称


    int row_num;//属性总数
    vector<tableRows> rows;//记录所有列属性的list
    int line_num;//记录数
    vector<string> lines;//表中的每行

    bool CreateTable(const vector<tableRows>& newTable,const string& tableName);//建立表
    bool saveToFile(save_mode mode=save_mode::CONTENT);//保存文件数据
    bool readFromFile(const string& tableName);//读取已经建造的表文件中的数据
public:
    Table(const string& tableName);//已存在表构造函数
    Table(const vector<tableRows>& newTable,const string& tableName);//创建表构造函数
    ~Table();
    void test();


    bool dropTable();//drop表
    bool instertTOTable( const string& content,const string &correspond="");

//    deleteTable
//    updateTable


};

#endif // TABLE_H

