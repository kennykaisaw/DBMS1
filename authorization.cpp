#include "authorization.h"

authorization::authorization()
{

}
bool authorization::createfolder(QString address_folder)
{
     QDir dir;
    if (!dir.exists(address_folder)) {
          // 如果不存在，创建文件夹
          if (dir.mkpath(address_folder)) {
              qDebug() << "Folder created successfully at" << address_folder;
              return true;
          } else {
              qDebug() << "Error creating folder.";
              return false;
          }
      } else {
          qDebug() << "Ruanko already exists at" << address_folder;
           return false;
      }
}
//默认在authorization下建立
bool authorization::createfile(string filename)
{
    string s_address_authorization = address_authorization.toStdString();
    //创建文件
    ofstream file_A(s_address_authorization+"\\"+filename+".bin",ios::binary);
    if (file_A.is_open())
    {
        cout<<"createFile"<<filename<<endl;
        file_A.close();
    }
    else
    {
        cerr << "Error opening file " <<filename<<endl;
        return false;
    }
    return true;
}

 bool authorization::readfromlogin()
 {
     string s_address_authorization = address_authorization.toStdString();
     ifstream file_login(s_address_authorization+"\\login.bin",ios::binary);
     //读取账号密码
     if (file_login.is_open())
     {
         string name;
         string password;
         while (file_login>>name>>password)
         {
             login_map.insert({name,password});
         }
         file_login.close();
     }
     return true;
 }
 bool authorization::open_usertable(string username)
 {
      string s_address_authorization = address_authorization.toStdString();
     ifstream file_user(s_address_authorization+"\\"+username+".bin",ios::binary);
     //读取账号密码
     if (file_user.is_open())
     {
         string name;
         string password;
         while (file_user>>name>>password)
         {
             login_map.insert({name,password});
         }
         file_user.close();
     }
     return true;
 }
bool authorization::initial()
{
     //建立authorization
    address_authorization  =   this->SYSTEMfolderPath +QDir::separator() + "authorization";
    createfolder(address_authorization);
    //建立login
    QString address_login =  address_authorization+QDir::separator() + "login";
    createfile(address_login.toStdString());
    //建立user_root,授予所有权限
    QString address_root =  address_authorization+QDir::separator() + "user_root";
    createfile(address_root.toStdString());
    //login里添加root信息
    string s_address_authorization = address_authorization.toStdString();
    ofstream file_A(s_address_authorization+"\\login.bin",ios::binary| std::ios::out);
    if (file_A.is_open())
    {
        string temp = "root 123456";
        file_A<<temp<<endl;
        file_A.close();
    }

    //没写
    grantpermission("all","Ruanko","root");

    return true;
}
bool authorization::user_register(string name,string password)
{
    //login里添加信息
    string s_address_authorization = address_authorization.toStdString();
    ofstream file_A(s_address_authorization+"\\login.bin",ios::binary| std::ios::out);
    if (file_A.is_open())
    {
        string temp = name+" "+password;
        file_A<<temp<<endl;
        file_A.close();
    }
    return true;
}
bool authorization::user_login(string name,string password)
{
    //读登录表
    readfromlogin();

    auto it = login_map.find(name);
        if (it != login_map.end())
        {
            // 找到了元素，比对密码
             if(it->second == password)
             {
                 cout<<"login successfully"<<endl;
             }
             else
             {
                 cerr<<"wrong password"<<endl;
             }
            return true;
        }
        // 没有找到元素
        cerr<<"no such user"<<endl;
        return false;

}
bool authorization::grantpermission(string permission_name,string dbname,string user)
{
    string s_address_authorization = address_authorization.toStdString();
    ofstream file_A(s_address_authorization+"\\"+user+".bin",ios::binary| std::ios::out);
    if (file_A.is_open())
    {

        file_A<<dbname<<"."<<permission_name<<endl;
        file_A.close();
    }
    return true;
}
 bool authorization::search_permission(string permission_name,string dbname)
 {
     string s_address_authorization = address_authorization.toStdString();
    ifstream file_user(s_address_authorization+"\\"+user_name+".bin",ios::binary);
    string db_permission= dbname+"."+permission_name;
    //读取权限
    if (file_user.is_open())
    {
        string have_permission;

        while (file_user>>have_permission)
        {
            if(db_permission == have_permission||have_permission=="Ruanko.all")
            {
                return true;
            }
        }
        file_user.close();
    }
     return false;
 }

