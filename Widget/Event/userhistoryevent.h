#ifndef USERHISTORYEVENT_H
#define USERHISTORYEVENT_H

#include <QWidget>

#include <QPainter>

#include "textlabel.h"
#include "historyeventtable.h"

class UserHistoryEvent : public QWidget
{
    Q_OBJECT
public:
    //历史事件控件
    //saveName指定保存路径，parent制定父对象即可
    explicit UserHistoryEvent(QString saveName = "", QWidget *parent = 0);

    void setGeometry(int x, int y, int w, int h);
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
    TextLabel  *_date;
    TextLabel  *_time0;
    TextLabel  *_detail;
public:
    HistoryEventTable   *table;
    
};

#endif // USERHISTORYEVENT_H
