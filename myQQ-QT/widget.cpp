#include "widget.h"
#include "toolbox1.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QColorDialog>


Widget:: Widget(const QIcon icon, int ID, QString name, toolbox1 *w, QWidget *parent)
    : QWidget(parent)
{
    userID = ID;
    username = name;
    main_w = w;
    setWindowTitle(name);
    setWindowIcon(icon);
    //设置最大化最小化按钮无效
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    //设置背景颜色
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor(Qt::darkCyan));
    setPalette(palette);

    init_widget();//初始化相关控件

}

Widget::~Widget()
{

}

void Widget::init_widget()
{
    textBrowser = new QTextBrowser;

    //设置textBrowser背景颜色或图片
    textBrowser->setStyleSheet("background-image: url(:/images/2.jpg);");
    //textBrowser->setStyleSheet("border-image: url(:/images/2.jpg);");

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(textBrowser);

    fontCombobox = new QFontComboBox;
    fontCombobox->setToolTip(tr("字体"));

    comboBox = new QComboBox;
    comboBox->setToolTip(tr("字号"));
    comboBox->setEditable(true);
    comboBox->addItem(QString::number(8));
    comboBox->addItem(QString::number(9));
    comboBox->addItem(QString::number(10));
    comboBox->addItem(QString::number(11));
    comboBox->addItem(QString::number(12));
    comboBox->addItem(QString::number(13));
    comboBox->addItem(QString::number(14));
    comboBox->addItem(QString::number(15));
    comboBox->addItem(QString::number(16));
    comboBox->addItem(QString::number(17));
    comboBox->addItem(QString::number(18));
    comboBox->addItem(QString::number(19));
    comboBox->addItem(QString::number(20));
    comboBox->addItem(QString::number(21));
    comboBox->addItem(QString::number(22));
    comboBox->setCurrentIndex(4);//设置当前索引号

    toolBtn_1 = new QToolButton;
    toolBtn_1->setToolTip(tr("加粗"));
    toolBtn_1->setText(tr("加粗"));
    toolBtn_1->setCheckable(true);
    toolBtn_1->setAutoRaise(true);
    toolBtn_1->setIcon(QPixmap(":/images/bold.png"));

    toolBtn_2 = new QToolButton;
    toolBtn_2->setToolTip(tr("倾斜"));
    toolBtn_2->setText(tr("倾斜"));
    toolBtn_2->setCheckable(true);
    toolBtn_2->setAutoRaise(true);
    toolBtn_2->setIcon(QPixmap(":/images/italic.png"));

    toolBtn_3 = new QToolButton;
    toolBtn_3->setToolTip(tr("下划线"));
    toolBtn_3->setText(tr("下划线"));
    toolBtn_3->setCheckable(true);
    toolBtn_3->setAutoRaise(true);
    toolBtn_3->setIcon(QPixmap(":/images/under.png"));

    toolBtn_4 = new QToolButton;
    toolBtn_4->setToolTip(tr("颜色"));
    toolBtn_4->setText(tr("颜色"));
    toolBtn_4->setCheckable(true);
    toolBtn_4->setAutoRaise(true);
    toolBtn_4->setIcon(QPixmap(":/images/color.png"));

    toolBtn = new QToolButton;
    toolBtn->setText(tr("显示主窗口"));
    toolBtn->setAutoRaise(true);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(fontCombobox);
    layout2->addWidget(comboBox);
    layout2->addWidget(toolBtn_1);
    layout2->addWidget(toolBtn_2);
    layout2->addWidget(toolBtn_3);
    layout2->addWidget(toolBtn_4);
    layout2->addWidget(toolBtn);

    lineEdit = new QLineEdit;

    pushBtn = new QPushButton;
    pushBtn->setText(tr("发送"));

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(lineEdit);
    layout3->addWidget(pushBtn);

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addLayout(layout1);
    mainlayout->addLayout(layout2);
    mainlayout->addLayout(layout3);
    //mainlayout->setSizeConstraint(QLayout::SetFixedSize);//设置窗口大小不能改变
    setLayout(mainlayout);

    textBrowser->setCurrentFont(fontCombobox->font());//设置textBrowser的默认字体
    textBrowser->setFontPointSize(comboBox->currentText().toDouble());//设置textBrowser的当前字号
    lineEdit->setFocus();

    connect(fontCombobox, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(on_fontComboBox_currentFontChanged(const QFont &)));
    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_currentIndexChanged(const QString &)));
    connect(toolBtn_1, SIGNAL(clicked(bool)), this, SLOT(on_toolButton_1_clicked(bool)));
    connect(toolBtn_2, SIGNAL(clicked(bool)), this, SLOT(on_toolButton_2_clicked(bool)));
    connect(toolBtn_3, SIGNAL(clicked(bool)), this, SLOT(on_toolButton_3_clicked(bool)));
    connect(toolBtn_4, SIGNAL(clicked()), this, SLOT(on_toolButton_4_clicked()));
    connect(toolBtn, SIGNAL(clicked()), this, SLOT(on_toolButton_clicked()));
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));
    connect(pushBtn, SIGNAL(clicked()), this, SLOT(on_pushBtnOnclick()));
}

void Widget::on_pushBtnOnclick()
{
    if(lineEdit->text().isEmpty())//如果lineEdit控件内容为空，提示用户不能发送空消息
    {
        QMessageBox::information(this,tr("注意"),tr("不能发送空消息"));
    }else
    {
        add_msg(tr("我的消息"),lineEdit->text());//将要发送的消息内容加入textBrowser空间中
        main_w->send_Msg(userID,lineEdit->text().toStdString().data());
        lineEdit->clear();//发送完成后，将lineEdit控件内容清空
    }
    lineEdit->setFocus();
}

void Widget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    textBrowser->setCurrentFont(f);
       textBrowser->setFontPointSize(comboBox->currentText().toDouble());

       if(toolBtn_1->isChecked())
       {
           textBrowser->setFontWeight(QFont::Bold);
       }
       else
       {
           textBrowser->setFontWeight(QFont::Normal);
       }

       if(toolBtn_2->isChecked())
       {
           textBrowser->setFontItalic(true);
       }
       else
       {
           textBrowser->setFontItalic(false);
       }

       if(toolBtn_3->isChecked())
       {
           textBrowser->setFontUnderline(true);
       }
       else
       {
           textBrowser->setFontUnderline(false);
       }

       textBrowser->setTextColor(color);
       lineEdit->setFocus();
}

void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    textBrowser->setFontPointSize(arg1.toDouble());
        lineEdit->setFocus();
}

void Widget::on_toolButton_1_clicked(bool checked)
{
    if(checked)
     {
         textBrowser->setFontWeight(QFont::Bold);
     }
     else
     {
         textBrowser->setFontWeight(QFont::Normal);
     }
     lineEdit->setFocus();

}

void Widget::on_toolButton_2_clicked(bool checked)
{
    textBrowser->setFontItalic(checked);
    lineEdit->setFocus();
}

void Widget::on_toolButton_3_clicked(bool checked)
{
    textBrowser->setFontUnderline(checked);
    lineEdit->setFocus();
}

void Widget::on_toolButton_4_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        textBrowser->setTextColor(color);
        lineEdit->setFocus();
    }
}

void Widget::on_lineEdit_returnPressed()
{
    if (pushBtn->isEnabled())//如果pushButton没有变灰，那么就可以调用on_pushButton_clicked()函数
        on_pushBtnOnclick();
}

void Widget::on_toolButton_clicked()
{
    main_w->hide();
    main_w->showNormal();
}


void Widget::add_msg(QString delivername,QString msg)//向textBrowser添加消息
{
    //得到当前时间，并把时间格式化为“yyyy-MM-dd hh:mi:ss"形式的字符串
    QString sTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    textBrowser->append("["+delivername+"]"+sTime);
    textBrowser->append(msg);

    //当消息textBrowser中消息过多出现滚动条时，自动滚动到最下方
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}



