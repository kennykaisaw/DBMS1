#include "db.h"

DB::DB()
{
    QString folderName = "Ruanko";

       // 创建一个QDir对象来管理文件夹
       QDir dir;
       // 获取当前工作目录
          QString currentPath = dir.currentPath();

          // 拼接文件夹的完整路径
          QString folderPath = currentPath + QDir::separator() + folderName;
       // 检查文件夹是否已经存在
       if (!dir.exists(folderPath)) {
           // 如果不存在，创建文件夹
           if (dir.mkdir(folderPath)) {
               qDebug() << "Folder created successfully."<<folderPath;
           } else {
               qDebug() << "Error creating folder.";
           }
       } else {
           qDebug() << "Folder already exists."<<folderPath;
       }
}
