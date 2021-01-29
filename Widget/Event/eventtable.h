#ifndef EVENTTABLE_H
#define EVENTTABLE_H

#include <QTableWidget>
#include <QScrollBar>
#include <QHeaderView>

//#include "universalModule/messageModule/messagebox.h"
//#include "languageTranslate/translate.h"

class EventTable : public QTableWidget
{
    Q_OBJECT
public:
    //事件记录表格类
    explicit EventTable(int column = 0,QWidget *parent = 0);

    //设置背景颜色
    void setBackgroundColor(QColor color);
    //设置背景图片
    void setBackgroundPixmap(QPixmap *pixmap);
    //插入一行记录
    //list是将要插入的内容，color控制显示的颜色
    void insertRow(QStringList list,QColor color);
    //设置记录的字号大小
    void setItemTextSize(int size);
    //设置某一行的颜色
    void setRowColor(int row, QColor color);

    //清楚事件记录显示(不会清除文件)
    void clearTabel();

    //设置条目显示空间不足时，以弹出框显示
    void setItemExpandShow(int column,bool ok);
private:
    void initTable(int column);
    void initMsg();
    void setEnglish();
    void setChinese();

private slots:
    void langChangeSlot();
    void retranslateUi(QLocale::Language lang);
    void itemPressSlot(const QModelIndex &index);

protected:
    void showEvent(QShowEvent *e);

private:
   // messageBox *privateMsg;
    QVector<int> _itemExpandShow;
    QFont _tFont;

public slots:

};

#endif // EVENTTABLE_H
