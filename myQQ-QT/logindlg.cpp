#include "loginDlg.h"
#include <QGridLayout>
#include <QMessageBox>

loginDlg::loginDlg(QWidget *parent) : QDialog(parent)
{
    label0 = new QLabel;
    label0->setText(tr("用户ID"));
    lineEditUerID = new QLineEdit;
    lineEditUerID->setText("1");

    label1 = new QLabel;
    label1->setText(tr("用户密码"));
    lineEditPassword = new QLineEdit;
    lineEditPassword->setEchoMode(QLineEdit::Password);//设置为密码框
    lineEditPassword->setText("123456");

    label2 = new QLabel;
    label2->setText(tr("服务器IP"));
    lineEditHostIP = new QLineEdit;
    lineEditHostIP->setText("123.207.163.170");

    label3 = new QLabel;
    label3->setText(tr("服务器端口号"));
    lineEditHostPort = new QLineEdit;
    lineEditHostPort->setText("8080");

    loginBtn = new QPushButton;
    loginBtn->setText(tr("登录"));
    logoutBtn = new QPushButton;
    logoutBtn->setText(tr("取消"));

    QGridLayout *layout1 = new QGridLayout(this);
    layout1->addWidget(label0,0,0);
    layout1->addWidget(lineEditUerID,0,1);
    layout1->addWidget(label1,1,0);
    layout1->addWidget(lineEditPassword,1,1);
    layout1->addWidget(label2,2,0);
    layout1->addWidget(lineEditHostIP,2,1);
    layout1->addWidget(label3,3,0);
    layout1->addWidget(lineEditHostPort,3,1);
    layout1->addWidget(loginBtn,4,0);
    layout1->addWidget(logoutBtn,4,1);

    layout1->setColumnStretch(0,1);
    layout1->setColumnStretch(1,1);

    layout1->setMargin(15);
    layout1->setSpacing(10);
    layout1->setSizeConstraint(QLayout::SetFixedSize);

    connect(loginBtn,SIGNAL(clicked(bool)),this,SLOT(loginBtnOnclick()));
    connect(logoutBtn,SIGNAL(clicked(bool)),this,SLOT(logoutBtnOnclick()));

    islogin = false;
    setWindowTitle(tr("myQQ v1.01"));
    setWindowIcon(QPixmap(":/images/3.png"));
    lineEditUerID->setFocus();//设置lineEditUserID空间具有输入焦点

    //设置窗口没有边框
    //setWindowFlags(Qt::FramelessWindowHint);

    //设置对话框背景颜色或图片
    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/images/1.jpg")));
    setPalette(palette);
}

void loginDlg::loginBtnOnclick()
{
    if(lineEditUerID->text().isEmpty())
    {
        QMessageBox::information(this,tr("错误"),tr("用户ID不能为空"));
        lineEditUerID->setFocus();
    }else
    {
        bool ok;
        userid = lineEditUerID->text().toInt(&ok);
        if(!ok)
        {
            QMessageBox::information(this,tr("错误"),tr("用户ID必须是从0-255的数字"));
            lineEditUerID->setFocus();
        }else
        {
            islogin = true;
            passwd = lineEditPassword->text();
            hostip = lineEditHostIP->text();
            hostport = lineEditHostPort->text().toInt();
            close();
        }
    }
}

void loginDlg::logoutBtnOnclick()
{
    close();
}
