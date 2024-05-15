#ifndef AUTORIZATION_H
#define AUTORIZATION_H
#include<iostream>
#include <QDir>
#include <QDebug>
#include <fstream>
#include<map>
using namespace std;

class autorization
{
private:
    bool createfolder(QString address_folder);
    bool createfile(string filename);
    bool readfromlogin();

public:
    //用户
    class user
    {

    public:
        string username;
    };
    //权限
    class permission
    {
    public:
    };

    autorization();
    //初始化anthortization文件夹和root用户
    bool initial();
    bool user_register(string name,string password);
    bool user_login(string name,string password);
    bool grantpermissoin();

    QString SYSTEMfolderPath = "D:/program/QT/DBMS/Ruanko";
    QString address_authorization;
    map<string ,string > login_map;


};

#endif // AUTORIZATION_H
