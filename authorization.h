#ifndef AUTORIZATION_H
#define AUTORIZATION_H
#include<iostream>
#include <QDir>
#include <QDebug>
#include <fstream>
#include<map>
using namespace std;

class authorization
{
private:
    bool createfolder(QString address_folder);
    bool createfile(string filename);
    bool readfromlogin();
    bool open_usertable(string username);

public:


    authorization();
    //初始化anthortization文件夹和root用户
    bool initial();
    //注册用户
    bool user_register(string name,string password);
    //登录用户
    bool user_login(string name,string password);
    //授权
    bool grantpermission(string permission_name,string dbname,string user);
    //搜索权限
    bool search_permission(string permission_name,string dbname);

    //用户存在authorization下面
    QString SYSTEMfolderPath = "D:/program/QT/DBMS/Ruanko";
    QString address_authorization;
    //账号，密码
    map<string ,string > login_map;
    //当前用户
    string user_name;


};

#endif // AUTORIZATION_H
