#ifndef USERCURRENTEVENT_H
#define USERCURRENTEVENT_H

#include <QWidget>
#include <QPainter>


#include "textlabel.h"
#include "currenteventtable.h"

class UserCurrentEvent : public QWidget
{
    Q_OBJECT
public:
    //当前事件记录
    //saveName指定保存目录名，parent指定父对象
    explicit UserCurrentEvent(QString saveName = "userEvent",QWidget *parent = 0);
    
    //设置控件坐标和大小
    //void setGeometry(int x, int y, int w, int h);

    //获取事件列表对象的地址，具体要如何配置事件记录的写入请看currentEventTable
    CurrentEventTable *tableHandle(){return table;}

private:
    void init();
    void setEnglish();
    void setChinese();
    void retranslateUi(QLocale::Language lang);

private slots:
    void langChangeSlot();

protected:
//    void paintEvent(QPaintEvent *e);

private:

    QPixmap *_bg;
    TextLabel  *_num;
    TextLabel  *_date0;
    TextLabel  *_date1;
    TextLabel  *_time0;
    TextLabel  *_time1;
    TextLabel  *_detail;
public:
        CurrentEventTable   *table;
public slots:
    
};

#endif // USERCURRENTEVENT_H
