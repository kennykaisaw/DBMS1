//#include "mainwindow.h"
//#include "ui_mainwindow.h"

//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
//}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}
#include "mainwindow.h"
#include "parser.h"
#include <QDebug>
#include <QString>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体显示的位置和大小 */
    this->setGeometry(0, 0, 1500, 900);

    /* 实例和对象，设置位置和显示大小 */
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(300, 200, 900, 665);

    /* 实例和对象，设置位置和显示大小，设置文本 */
    pushButtonDownloadAll = new QPushButton(this);
    pushButtonDownloadAll->setGeometry(1300, 635, 100, 70);
    pushButtonDownloadAll->setText("确定");
    pushButtonDownloadAll->setStyleSheet("font-size: 17pt;");

    /* 实例和对象，设置位置和显示大小，设置文本 */
    pushButtonClearAll = new QPushButton(this);
    pushButtonClearAll->setGeometry(1300, 730, 100, 70);
    pushButtonClearAll->setText("清除");
    pushButtonClearAll->setStyleSheet("font-size: 17pt;");

    /* 实例和对象，设置位置和显示大小，设置文本 */
    pushButtonSelectAll = new QPushButton(this);
    pushButtonSelectAll->setGeometry(1300, 540, 100, 70);
    pushButtonSelectAll->setText("全选");
    pushButtonSelectAll->setStyleSheet("font-size: 17pt;");

    pushButtonFile = new QPushButton(this);
    pushButtonFile->setGeometry(0, 0, 65, 40);
    pushButtonFile->setText("文件");
    pushButtonEdit = new QPushButton(this);
    pushButtonEdit->setGeometry(65, 0, 65, 40);
    pushButtonEdit->setText("编辑");
    pushButtonLook = new QPushButton(this);
    pushButtonLook->setGeometry(130, 0, 65, 40);
    pushButtonLook->setText("查看");
    pushButtonStar = new QPushButton(this);
    pushButtonStar->setGeometry(195, 0, 65, 40);
    pushButtonStar->setText("收藏夹");
    pushButtonTool = new QPushButton(this);
    pushButtonTool->setGeometry(260, 0, 65, 40);
    pushButtonTool->setText("工具");
    pushButtonWindow = new QPushButton(this);
    pushButtonWindow->setGeometry(325, 0, 65, 40);
    pushButtonWindow->setText("窗口");




    pushButtonConnect = new QPushButton(this);
    pushButtonConnect->setGeometry(0, 55, 125, 130);
    pushButtonConnect->setText("连接");
    pushButtonConnect->setStyleSheet("padding-bottom: -95px;");
    QLabel *label1 = new QLabel(this);
    label1->setGeometry(0, 55, 125, 100);
    QPixmap pixmap1("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\连接.png");
    label1->setPixmap(pixmap1);
    label1->setScaledContents(true);


    pushButtonNewAsk = new QPushButton(this);
    pushButtonNewAsk->setGeometry(125, 55, 125, 130);
    pushButtonNewAsk->setText("新建查询");
    pushButtonNewAsk->setStyleSheet("padding-bottom: -95px;");
    QLabel *label2 = new QLabel(this);
    label2->setGeometry(125, 55, 125, 100);
    QPixmap pixmap2("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\新建查询.png");
    label2->setPixmap(pixmap2);
    label2->setScaledContents(true);

    pushButtonExcel = new QPushButton(this);
    pushButtonExcel->setGeometry(250, 55, 125, 130);
    pushButtonExcel->setText("表");
    pushButtonExcel->setStyleSheet("padding-bottom: -95px;");
    QLabel *label3 = new QLabel(this);
    label3->setGeometry(250, 55, 125, 100);
    QPixmap pixmap3("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\表.png");
    label3->setPixmap(pixmap3);
    label3->setScaledContents(true);

    pushButtonPicture = new QPushButton(this);
    pushButtonPicture->setGeometry(375, 55, 125, 130);
    pushButtonPicture->setText("视图");
    pushButtonPicture->setStyleSheet("padding-bottom: -95px;");
    QLabel *label4 = new QLabel(this);
    label4->setGeometry(375, 55, 125, 100);
    QPixmap pixmap4("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\视图.png");
    label4->setPixmap(pixmap4);
    label4->setScaledContents(true);

    pushButtonFx = new QPushButton(this);
    pushButtonFx->setGeometry(500, 55, 125, 130);
    pushButtonFx->setText("函数");
    pushButtonFx->setStyleSheet("padding-bottom: -95px;");
    QLabel *label5 = new QLabel(this);
    label5->setGeometry(500, 55, 125, 100);
    QPixmap pixmap5("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\函数.png");
    label5->setPixmap(pixmap5);
    label5->setScaledContents(true);

    pushButtonUser = new QPushButton(this);
    pushButtonUser->setGeometry(625, 55, 125, 130);
    pushButtonUser->setText("用户");
    pushButtonUser->setStyleSheet("padding-bottom: -95px;");
    QLabel *label6 = new QLabel(this);
    label6->setGeometry(625, 55, 125, 100);
    QPixmap pixmap6("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\用户.png");
    label6->setPixmap(pixmap6);
    label6->setScaledContents(true);

    pushButtonOther = new QPushButton(this);
    pushButtonOther->setGeometry(750, 55, 125, 130);
    pushButtonOther->setText("其它");
    pushButtonOther->setStyleSheet("padding-bottom: -95px;");
    QLabel *label7 = new QLabel(this);
    label7->setGeometry(750, 55, 125, 100);
    QPixmap pixmap7("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\其它.png");
    label7->setPixmap(pixmap7);
    label7->setScaledContents(true);

    pushButtonAsk = new QPushButton(this);
    pushButtonAsk->setGeometry(875, 55, 125, 130);
    pushButtonAsk->setText("查询");
    pushButtonAsk->setStyleSheet("padding-bottom: -95px;");
    QLabel *label8 = new QLabel(this);
    label8->setGeometry(875, 55, 125, 100);
    QPixmap pixmap8("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\查询.png");
    label8->setPixmap(pixmap8);
    label8->setScaledContents(true);

    pushButtonAnother = new QPushButton(this);
    pushButtonAnother->setGeometry(1000, 55, 125, 130);
    pushButtonAnother->setText("备份");
    pushButtonAnother->setStyleSheet("padding-bottom: -95px;");
    QLabel *label9 = new QLabel(this);
    label9->setGeometry(1000, 55, 125, 100);
    QPixmap pixmap9("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\备份.png");
    label9->setPixmap(pixmap9);
    label9->setScaledContents(true);

    pushButtonAutoDo = new QPushButton(this);
    pushButtonAutoDo->setGeometry(1125, 55, 125, 130);
    pushButtonAutoDo->setText("自动运行");
    pushButtonAutoDo->setStyleSheet("padding-bottom: -95px;");
    QLabel *label10 = new QLabel(this);
    label10->setGeometry(1125, 55, 125, 100);
    QPixmap pixmap10("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\自动运行.png");
    label10->setPixmap(pixmap10);
    label10->setScaledContents(true);

    pushButtonModel = new QPushButton(this);
    pushButtonModel->setGeometry(1250, 55, 125, 130);
    pushButtonModel->setText("模型");
    pushButtonModel->setStyleSheet("padding-bottom: -95px;");
    QLabel *label11 = new QLabel(this);
    label11->setGeometry(1250, 55, 125, 100);
    QPixmap pixmap11("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\模型.png");
    label11->setPixmap(pixmap11);
    label11->setScaledContents(true);

    pushButtonPictureExcel = new QPushButton(this);
    pushButtonPictureExcel->setGeometry(1375, 55, 125, 130);
    pushButtonPictureExcel->setText("图表");
    pushButtonPictureExcel->setStyleSheet("padding-bottom: -95px;");
    QLabel *label12 = new QLabel(this);
    label12->setGeometry(1375, 55, 125, 100);
    QPixmap pixmap12("C:\\Users\\Lucas\\Desktop\\DBMS1-master\\materials\\图表.png");
    label12->setPixmap(pixmap12);
    label12->setScaledContents(true);
    //k
    db->createRuanko();
    //k


    /* 信号槽连接 */
    connect(pushButtonSelectAll, SIGNAL(clicked()), this,
            SLOT(pushButtonSelectAllClicked()));
    connect(pushButtonClearAll, SIGNAL(clicked()), this,
            SLOT(pushButtonClearAllClicked()));
    connect(pushButtonDownloadAll, SIGNAL(clicked()), this,
            SLOT(pushButtonDownloadAllClicked()));

}

MainWindow::~MainWindow()
{
    delete db;
}

void MainWindow::pushButtonSelectAllClicked()
{
    /* 设置焦点为textEdit */
    textEdit->setFocus();
    /* 判断文本编辑框内容是否为空，不为空则全选 */
    if(!textEdit->toPlainText().isEmpty()){
        /* 全选 */
        textEdit->selectAll();
    }
}

void MainWindow::pushButtonDownloadAllClicked()
{
    Parser p(db);

    /* 设置焦点为textEdit */
    textEdit->setFocus();
    /* 判断文本编辑框内容是否为空，不为空则全选 */
    if(!textEdit->toPlainText().isEmpty()){
        /* 保存 */
        // 提取QTextEdit中的纯文本内容
        QString content = textEdit->toPlainText();
        //p.parsermulty(content);
        p.parserfirst(content);
        //p.insert_table(content);
        qDebug() << content;
        textEdit->clear();
    }
}

void MainWindow::pushButtonClearAllClicked()
{
    /* 清空textEdit里的文本内容 */
    textEdit->clear();
}


