//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H

//#include <QMainWindow>
//#include<QFileSystemModel>
//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

//class MainWindow : public QMainWindow
//{
//    Q_OBJECT

//public:
//    MainWindow(QWidget *parent = nullptr);
//    ~MainWindow();

//private:
//    Ui::MainWindow *ui;
//};
//#endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTextEdit>
#include <QPushButton>

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QTextEdit对象 */
    QTextEdit *textEdit;

    /* 声明两个QPushButton对象 */
    QPushButton *pushButtonSelectAll;
    QPushButton *pushButtonDownloadAll;
    QPushButton *pushButtonClearAll;
    QPushButton *pushButtonFile;
    QPushButton *pushButtonEdit;
    QPushButton *pushButtonLook;
    QPushButton *pushButtonStar;
    QPushButton *pushButtonTool;
    QPushButton *pushButtonWindow;
    QPushButton *pushButtonHelp;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonNewAsk;
    QPushButton *pushButtonExcel;
    QPushButton *pushButtonPicture;
    QPushButton *pushButtonFx;
    QPushButton *pushButtonUser;
    QPushButton *pushButtonOther;
    QPushButton *pushButtonAsk;
    QPushButton *pushButtonAnother;
    QPushButton *pushButtonAutoDo;
    QPushButton *pushButtonModel;
    QPushButton *pushButtonPictureExcel;

private slots:
    /* 声明两个槽函数，响应按钮点击响应的事件 */
    void pushButtonSelectAllClicked();
    void pushButtonDownloadAllClicked();
    void pushButtonClearAllClicked();

};
#endif // MAINWINDOW_H
