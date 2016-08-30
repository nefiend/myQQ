#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QFontComboBox>
#include <QComboBox>
#include <QToolButton>
#include <QLineEdit>
#include <QColor>

class toolbox1;//为了防止toolbox和widget头文件交叉包含，因此在widget中先声明一个toolbox1类，然后在cpp文件中包含toolbox1.h，可以避免这种情况

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(const QIcon icon, int ID, QString name, toolbox1 *w, QWidget *parent = 0);
    ~Widget();

    QPushButton *pushBtn;
    void add_msg(QString delivername,QString msg);//向textBrowser添加消息
private:
    void init_widget();

    QTextBrowser *textBrowser;
    QFontComboBox *fontCombobox;
    QComboBox *comboBox;
    QToolButton *toolBtn_1,*toolBtn_2,*toolBtn_3,*toolBtn_4,*toolBtn;

    QLineEdit *lineEdit;



    int userID;
    QString username;
    QColor color;
    toolbox1 *main_w;

private slots:
    void on_pushBtnOnclick();
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_toolButton_1_clicked(bool checked);
    void on_toolButton_2_clicked(bool checked);
    void on_toolButton_3_clicked(bool checked);
    void on_toolButton_4_clicked();
    void on_lineEdit_returnPressed();
    void on_toolButton_clicked();


};



#endif // WIDGET_H
