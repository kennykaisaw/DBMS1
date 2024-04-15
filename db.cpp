#include "db.h"

DB::DB()
{




}
  //系统数据库，不能删
bool DB::createRuanko()
{
    // 创建一个QDir对象来管理文件夹
    QDir dir;

    // 指定要创建的文件夹的绝对路径（Windows示例）,ruanko是要创建的文件夹
       QString SYSTEMfolderPath = "D:/program/QT/DBMS/Ruanko";



       if (!dir.exists(SYSTEMfolderPath)) {
             // 如果不存在，创建文件夹
             if (dir.mkpath(SYSTEMfolderPath)) {
                 qDebug() << "Folder created successfully at" << SYSTEMfolderPath;
                 return true;
             } else {
                 qDebug() << "Error creating folder.";
                 return false;
             }
         } else {
             qDebug() << "Folder already exists at" << SYSTEMfolderPath;
              return false;
         }

}
//用户数据库
bool DB::createUserdb(QString USERdbName)
{

    // 创建一个QDir对象来管理文件夹
    QDir dir;


      //need ui;
      //std::cin>>USERdbName;

   // 指定要创建的文件夹的绝对路径（Windows示例）
      QString currentdbPath = "D:/program/QT/DBMS";

      // 拼接文件夹的完整路径
         QString USERdbPath = currentdbPath + QDir::separator() + USERdbName;



      if (!dir.exists(USERdbPath)) {
            // 如果不存在，创建文件夹
            if (dir.mkpath(USERdbPath)) {
                qDebug() << "Folder created successfully at" << USERdbPath;
                return true;
            } else {
                qDebug() << "Error creating folder.";
                return false;
            }
        } else {
            qDebug() << "Folder already exists at" << USERdbPath;
            return false;
        }

}
 bool DB::usedb(QString db)
 {
    this->currentdb = db;
     return true;
 }
