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
        this->SYSTEMfolderPath = "D:/program/QT/DBMS/Ruanko";



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
             qDebug() << "Ruanko already exists at" << SYSTEMfolderPath;

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
      QString currentdbPath = SYSTEMfolderPath;

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
     QDir dir;
     if (dir.exists(SYSTEMfolderPath+ QDir::separator()+db))
     {
         this->CurrentDbPath = SYSTEMfolderPath+ QDir::separator()+db;
         qDebug() << "use"<<db<<"successfully";
         return true;
     }
     else
     {
         qDebug() << "not exist this database";
         return false;
     }
 }
 bool DB::selectdatabase()
 {
      qDebug() << this->CurrentDbPath;
      return true;
 }
 bool DB::showdbs()
 {

         QString folderPath = "D:/program/QT/DBMS/Ruanko";

         QDir dir(folderPath);

         // 获取指定文件夹下的所有文件和文件夹的名称列表
             QStringList entryList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

             // 遍历列表，找出所有文件夹的名称

             for (const QString &entry : entryList) {
                 // 构造文件夹路径
                 QString entryPath = folderPath + "/" + entry;

                 // 创建 QFileInfo 对象
                 QFileInfo fileInfo(entryPath);

                 // 判断是否是文件夹
                 if (fileInfo.isDir()) {
                     // 添加文件夹名称到列表中
                     dbs.append(entry);
                 }
             }


     for(int i = 0; i < dbs.size(); ++i) {
            qDebug() << dbs[i];
        }
     return true;

 }

