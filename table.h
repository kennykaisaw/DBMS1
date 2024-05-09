#ifndef TABLE_H
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
#include"db.h"
using namespace std;



enum type
{
    INT,DOUBLE,VARCHAR,CHAR
};

enum constrain{NONE=0};

struct tableRows//要给一个静态变量int
{
    string rowName;//列名
    type rowType;//列的数据要求INT
    int typeLen;//长度要求
    constrain rowConstrain;//列的限制
    bool isPrimaryKey;//是否主键
    //string forignKeyName;//外键名称//多个外键怎么办？？？？？？？？？？？？？
    //string forignKeyTable;//外键所属表名称
    string content;
};//简历表信息的结构体


class Table
{
    static const char delimiter = '\n';//文件写入分隔符
    enum class save_mode{CONTENT=-1,ATTRIBUTE=1,BOTH=0};//保存文件的选项
    enum class delete_mode{ALL=-1,SELECT=0};
    enum class checkLines_mode{LINE=0};
private:
    string add;//文件夹保存地址
    string tableName;//表名称


    int row_num;//属性总数
    vector<tableRows> rows;//记录所有列属性的list
    int line_num;//记录数
    vector<string> lines;//表中的每行
    vector<tableRows> this_row;//当前的一行

    bool CreateTable(const vector<tableRows>& newTable,const string& tableName);//建立表
    bool saveToFile(save_mode mode=save_mode::CONTENT);//保存文件数据
    bool readFromFile(const string& tableName);//读取已经建造的表文件中的数据
    string getShowType(const string& shows) //展示转换函数
    {
        // 检查字符串是否以单引号开头和结尾
        if (shows.length() > 1 && shows[0] == '\'' && shows[shows.length() - 1] == '\'')
        {
            // 去除字符串两端的单引号
            return shows.substr(1, shows.length() - 2);
        }
        else
        {
            // 如果不是，直接返回原字符串
            return shows;
        }
    }
    vector<string> splitByPipe(const string& input) //分割字符串|
    {
        vector<string> result;
        istringstream iss(input);
        string token;
        while (std::getline(iss, token, '|'))
        {
            result.push_back(token);
        }

        return result;
    }
    bool checkType(const int& row_index,const string& content)//检查对应类型
    {
        return true;
    }//！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！这个没写呢！！！！！！！！！
    void checkLines(string& in,checkLines_mode mode=checkLines_mode::LINE);//将in的内容放在this_line中进行操作
public:
    //kenny
    Table(const string& tableName,DB * db);//已存在表构造函数
    Table(const vector<tableRows>& newTable,const string& tableName);//创建表构造函数
    //kenny
     Table(const vector<tableRows>& newTable,const string& tableName,DB * db);//表构造函数，需要db

    DB *db;
    //kenny
    ~Table();
    void test();


    bool dropTable();//drop表
    bool instertTOTable( const string& content,const string &correspond="");
    bool deleteFromTable(delete_mode=delete_mode::ALL);


    void show();



//    updateTable


};

#endif // TABLE_H
