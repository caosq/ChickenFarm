#include "eventtable.h"

#define DEF_TEXT_SIZE 14

EventTable::EventTable(int column, QWidget *parent) :
    QTableWidget(parent)
{
    _tFont.setPointSize(DEF_TEXT_SIZE);
    _itemExpandShow.resize(column);
    _itemExpandShow.fill(0);

    setAutoFillBackground(true);

    initTable(column);

    initMsg();

    //retranslateUi(translate::instance()->currentLanguage());

    connect(this,SIGNAL(pressed(QModelIndex)),this,SLOT(itemPressSlot(QModelIndex)));
}

void EventTable::itemPressSlot(const QModelIndex &index)
{
    QString temp;
    temp = index.data().toString();

    QFontMetrics metrics(_tFont);

    if(metrics.width(temp) < visualItemRect(item(index.row(),index.column())).width()
                || 0 == _itemExpandShow[index.column()] )
    {
        return;
    }
    else
    {
//        privateMsg->setInformativeText(temp);
//        privateMsg->show();
    }
}

void EventTable::setItemExpandShow(int column, bool ok)
{
    column = column - 1;
    if( column < _itemExpandShow.size())
    {
        if(ok)
            _itemExpandShow[column] = 1;
        else
            _itemExpandShow[column] = 0;
    }
}

void EventTable::setRowColor(int row, QColor color)
{
    if( row > rowCount() && row < 0)
    {
        qDebug("row overflow");
        return;
    }

    for(int i = 0; i < columnCount(); i++)
    {
        item(row, i)->setTextColor(color);
    }
}

void EventTable::clearTabel()
{
    clear();
    int max = rowCount();
    for(int i = 0; i < max; i++ )
    {
        removeRow(0);
    }
}

void EventTable::setBackgroundColor(QColor color)
{
    QPalette palette;
    palette.setBrush(QPalette::Active,QPalette::Base,QBrush(color));
    palette.setBrush(QPalette::Inactive,QPalette::Base,QBrush(color));
    setPalette(palette);
}

void EventTable::setBackgroundPixmap(QPixmap *pixmap)
{
    QPixmap bg = pixmap->scaled(this->width(),this->height());

    QPalette palette;
    palette.setBrush(QPalette::All,QPalette::Base,QBrush(bg));
    palette.setBrush(QPalette::All,QPalette::Background,QBrush(bg));
    setPalette(palette);
}

void EventTable::initTable(int column)
{
    setFocusPolicy(Qt::NoFocus);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    horizontalHeader()->setStretchLastSection(true);

    setGridStyle(Qt::NoPen);
    setFrameShape(QFrame::NoFrame);

    //setBackgroundColor(Qt::transparent);

    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);

    verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                      "{"
                      "width:26px;"
                      "border-image:url(:/images/baseFile/event_scbg.png);"
                      "margin:0px 0px 0px 0px;"
                      "}"
                      "QScrollBar::handle:vertical"
                      "{"
                      "width:18px;"
                      "border-image:url(:/images/baseFile/event_sch.png);"
                      "margin:0px 0px 0px 0px;"
                      "min-height:60px;"
                      "}"
                      "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                      "{"
                      "background:none;"
                      "}"
                      "QScrollBar::add-line:vertical"
                      "{"
                      "height:0px;"
                      "width:0px;"
                      "}"
                      "QScrollBar::sub-line:vertical"
                      "{"
                      "height:0px;"
                      "width:0px;"
                      "}"
                  );
    setColumnCount(column);
}

void EventTable::initMsg()
{
//    privateMsg = new messageBox;
}

void EventTable::setEnglish()
{
//    privateMsg->setButtonText(messageBox::Yes,"Close");
}

void EventTable::setChinese()
{
//    privateMsg->setButtonText(messageBox::Yes,"å…³é—­");
}

void EventTable::langChangeSlot()
{
//    retranslateUi(translate::instance()->currentLanguage());
}

void EventTable::retranslateUi(QLocale::Language lang)
{
    switch(lang)
    {
        case QLocale::Chinese:
    {
        setChinese();
    }
        break;
    case QLocale::English:
    {
        setEnglish();
    }
        break;
    default:
        break;
    }
}

void EventTable::showEvent(QShowEvent *e)
{
    QTableWidget::showEvent(e);
    int minVal = verticalScrollBar()->minimum();
    verticalScrollBar()->setValue(minVal);
}

void EventTable::insertRow(QStringList list, QColor color)
{
    QTableWidget::insertRow(0);
    QTableWidgetItem *item;

    int size = qMin(list.size(),columnCount());
    for(int i = 0; i < size; i++)
    {
        item = new QTableWidgetItem(list.at(i),i);
        item->setTextAlignment(Qt::AlignLeft);
        item->setTextColor(color);
        item->setFont(_tFont);
        setItem(0,i,item);
    }
}

void EventTable::setItemTextSize(int size)
{
    _tFont.setPointSize(size);
}
