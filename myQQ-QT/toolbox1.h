#ifndef TOOLBOX1_H
#define TOOLBOX1_H

#include <QToolBox>
#include <QToolButton>
#include <QEvent>
#include <QtNetwork>
#include <QTcpSocket>
#include <QPushButton>
#include "widget.h"


#define CLIENTCOUNT 256 //最大客户端数量
#define MSGBODYBUF 1024

//定义消息结构
struct msg_t
{
    unsigned char head[4];
    char body[MSGBODYBUF];
};

class toolbox1 : public QToolBox
{
    Q_OBJECT
public:
    explicit toolbox1(QWidget *parent = 0);
    void send_Msg(int d_userid,const char *msgBody);//发往server的send消息

signals:

public slots:
    bool eventFilter(QObject *watched, QEvent *event);//第一个参数是在哪儿触发了消息，第二个参数是什么消息

private:
    int userid;
    QString passwd;
    QString hostip;
    int hostport;

    QString username[CLIENTCOUNT];
    QToolButton *toolBtn[CLIENTCOUNT];//创建256个toolbutton
    Widget *child[CLIENTCOUNT];//创建256个widget
    QToolButton *logoutBtn;

    void init_toolBtn();
    void init_username();
    QTcpSocket *sockClient;
    void login_Msg();

private slots:
    void sock_Error(QAbstractSocket::SocketError sockErr);//socket出错的时候触发的槽函数
    void read_Msg();//socket接收到消息触发的槽函数
    void recv_send_Msg(int userid,const char *msgbody);
    void userStatus_Msg(const char *msgBody);
    void system_Msg(int msgcode);
    void socket_connected();//socket成功连接到服务端后触发的槽函数
};

#endif // TOOLBOX1_H
