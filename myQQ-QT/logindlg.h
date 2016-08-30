#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


class loginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit loginDlg(QWidget *parent = 0);

signals:

public slots:

private:
    QLabel *label0,*label1,*label2,*label3;
    QLineEdit *lineEditUerID,*lineEditPassword,*lineEditHostIP,*lineEditHostPort;
    QPushButton *loginBtn,*logoutBtn;

private slots:
    void loginBtnOnclick();
    void logoutBtnOnclick();
public:
    int userid;
    QString passwd;
    QString hostip;
    int hostport;
    bool islogin;


};

#endif // loginDlg_H
