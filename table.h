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
#include <limits>
#include <stack>
#include <QDate>
#include"constrain.h"
#include"db.h"
using namespace std;



enum type
{
    INT,DOUBLE,CHAR,DATE
};


struct tableRows//要给一个静态变量int
{
    string rowName;//列名
    type rowType;//列的数据要求INT
    bool isPrimaryKey=false;//是否主键
    string forignKeyName="";//外键名称//多个外键怎么办？？？？？？？？？？？？？
    string forignKeyTable="";//外键所属表名称
    string default_content="";//默认值
    string constrainMessage="";//列的限制
    string content="";
    tableRows(string rowNames,type rowType
             ,bool isPrimaryKey=false, string forignKeyName=""
            ,string forignKeyTable="",string default_content="",string constrainMessage="",string content=""):
    rowName(rowNames),rowType(rowType),
    isPrimaryKey(isPrimaryKey),forignKeyName(forignKeyName),
    forignKeyTable(forignKeyTable),default_content(default_content) ,constrainMessage(constrainMessage),content(content)
    {}
    tableRows()=default;
};//简历表信息的结构体


class Table
{
    static const char delimiter = '\n';//文件写入分隔符
    enum class save_mode{CONTENT=-1,ATTRIBUTE=1,BOTH=0};//保存文件的选项
    enum class delete_mode{ALL=-1,SELECT=0};
    enum class alter_mode{DROP,ADD,RENAME,MODIFY,ALTER};
    enum class alter_class{ROW,PRIMARY_KRY,CONSTRAIN,TABLE};
   //DROP+ROW,PRIMARY_KRY,CONSTRAIN
    //ADD+ROW,CONSTRAIN      后续补充INDEX
    //RENAME+ROW,TABLE
    //MODIFY+ROW（新属性）
    //ALTER+ROW （set default）


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
 public:   bool checkType(const type& types,const string& content)//检查对应类型
    {
        if(types==type::INT)
        {
            int result;
            std::istringstream iss(content);
            iss >> std::noskipws >> result; // std::noskipws 用来处理前导空格
            // 如果流中没有错误，并且已经到了流的末尾，则转换成功
            return !iss.fail() && iss.eof();
        }
        else if(types==type::DOUBLE)
        {
            double result;
            std::istringstream iss(content);
            iss >> result;
            // 如果流中没有错误，并且已经到了流的末尾（或者下一个字符无法转换为double），则转换成功
            // 注意：如果字符串中包含非数字字符后的数字（如 "123abc456"），iss >> result 仍然会成功，但只会转换到第一个非数字字符
            return !iss.fail() && iss.eof() ;
        }
        else if(types==type::CHAR)
        {
            // 检查字符串长度是否至少为2（因为需要至少一个字符来开头和结尾）
            if (content.length() < 2)
            {
                return false;
            }
            // 检查第一个和最后一个字符是否是单引号
            return (content.front() == '\'' && content.back() == '\'');
        }
        else if(types==type::DATE)
        {
            QString content_v = QString::fromStdString(content);
            QDate date = QDate::fromString(content_v,"yyyy-MM-dd");
            return date.isValid();
        }
        else return false;
    }
    void checkLines(const string& in);//将in的内容放在this_line中进行操作


    //多重and or检测
    bool evaluate(const std::string& expr, const std::vector<bool>& values)
    {
        std::stack<bool> operands;
        std::stack<char> operators;
        int valueIndex = 0;

        auto applyOperator = [](bool a, bool b, char op) -> bool
            {
            if (op == 'A') return a && b;
            if (op == 'O') return a || b;
            return false;
            };

        for (int i = 0; i < expr.length(); ++i) {
            char c = expr[i];

            if (c == '*')
            {
                operands.push(values[valueIndex++]);
            }
            else if (c == '(')
            {
                operators.push(c);
            }
            else if (c == ')')
            {
                while (!operators.empty() && operators.top() != '(')
                {
                    bool b = operands.top(); operands.pop();
                    bool a = operands.top(); operands.pop();
                    char op = operators.top(); operators.pop();
                    operands.push(applyOperator(a, b, op));
                }
                operators.pop(); // Remove '('
            }
            else if (c == 'A' || c == 'O')
            {
                while (!operators.empty() && operators.top() != '(' &&
                    ((c == 'O' && operators.top() == 'A') || (c == operators.top())))
                {
                    bool b = operands.top(); operands.pop();
                    bool a = operands.top(); operands.pop();
                    char op = operators.top(); operators.pop();
                    operands.push(applyOperator(a, b, op));
                }
                operators.push(c);
            }
        }

        while (!operators.empty()) {
            bool b = operands.top(); operands.pop();
            bool a = operands.top(); operands.pop();
            char op = operators.top(); operators.pop();
            operands.push(applyOperator(a, b, op));
        }

        return operands.top();
    }
    //模板的定义要放在头文件里
public:template<class inPut>
    bool translateFromConstrainMessageAndCheck(const string& message,const type&type,const inPut &inputs)
    {
        istringstream iss(message);
        string line;
        int n;
        string content;
        vector<std::string> strings;

        // 读取第一行：整数n
        if (!(iss >> n))
        {
            std::cerr << "Invalid input: could not read the first line (integer n)." << std::endl;
            return false;
        }

        // 忽略第一行后面的换行符
        iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // 读取第二行：字符串content
        if (!std::getline(iss, content))
        {
            std::cerr << "Invalid input: could not read the second line (content)." << std::endl;
            return false;
        }

        // 读取第三行及以后的n个字符串
        for (int i = 0; i < n && std::getline(iss, line); ++i)
        {
            strings.push_back(line);
        }

        // 检查是否读取了足够的字符串
        if (strings.size() != (unsigned int)n)
        {
            std::cerr << "Invalid input: did not read the expected number of strings." << std::endl;
            return false;
        }

        //对每一个限制进行处理
        vector<bool>checks;
        for(unsigned int i=0;i<(unsigned int)n;++i)
        {
            vector<string> a=splitByPipe(strings[i]);
            if(a[0]=="daYu")
            {
                if(!checkType(type,a[1]))
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }

                daYu<inPut> c(static_cast<inPut>(stoi(a[1])));
                checks.push_back(c(inputs));
            }
            else if(a[0]=="xiaoYu")
            {

                if(!checkType(type,a[1]))
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }

                xiaoYu<inPut> c(static_cast<inPut>(stoi(a[1])));
                checks.push_back(c(inputs));
            }
            else if(a[0]=="dengYu")
            {
                if(!checkType(type,a[1]))
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }

                dengYu<inPut> c(static_cast<inPut>(stoi(a[1])));
                checks.push_back(c(inputs));

            }
            else if(a[0]=="xiaoYuDengYu")
            {
                if(!checkType(type,a[1]))
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }
                xiaoYuDengYu<inPut> c(static_cast<inPut>(stoi(a[1])));
                checks.push_back(c(inputs));
            }
            else if(a[0]=="daYuDengYu")
            {

                if(!checkType(type,a[1]))
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }
                daYuDengYu<inPut> c(static_cast<inPut>(stoi(a[1])));
                checks.push_back(c(inputs));
            }
            else if(a[0]=="between")
            {
                if(!checkType(type,a[1])||!checkType(type,a[2]))
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }
                between<inPut> c(static_cast<inPut>(stoi(a[1])),static_cast<inPut>(stoi(a[2])));
                checks.push_back(c(inputs));
            }
            else if(a[0]=="in")
            {
                bool flag=true;
                vector<inPut> list;
                for(unsigned int j=2;j<=(unsigned int)n+1;++j)
                {
                    flag*=checkType(type,a[j]);
                    list.push_back(static_cast<inPut>(stoi(a[j])));
                }
                if(!flag)
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }
                in<inPut> c(list);
                checks.push_back(c(inputs));
            }
            else if(a[0]=="notIn")
            {
                bool flag=true;
                vector<inPut> list;
                for(unsigned int j=2;j<=(unsigned int)n+1;++j)
                {
                    flag*=checkType(type,a[j]);
                    list.push_back(static_cast<inPut>(stoi(a[j])));
                }
                if(!flag)
                {
                    cerr<<"constrain type not match！"<<endl;
                    return false;
                }
                notIn<inPut> c(list);
                checks.push_back(c(inputs));
            }
            else
            {
                cerr<<"no such constrain"<<endl;
                return false;
            }
        }
        return evaluate(content,checks);
    }//翻译限制消息，检查限制信息
public:
    //kenny
    Table(const string& tableName,DB * db);//已存在表构造函数
    Table(const vector<tableRows>& newTable,const string& tableName);//创建表构造函数
    //kenny
     Table(const vector<tableRows>& newTable,const string& tableName,DB * db);//表构造函数，需要db
     //kenny
     bool selectfrom(const vector<string>& selectedrows ,const string& tableName); //查询
      bool selectallfrom(const string& tableName); //查询*
     //kenny

    DB *db;
    //kenny
    ~Table();
    void test();


    bool dropTable();//drop表
    bool instertTOTable( const string& content,const string &correspond="");
    bool deleteFromTable(const delete_mode=delete_mode::ALL);
    bool updateTable(const vector<string>&rowname,const vector<string>&goal/*,constrains*/);
    bool alterTable(const alter_mode& mode,const alter_class& class_A,const string& content);


    void show();





};

#endif // TABLE_H
