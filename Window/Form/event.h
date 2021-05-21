#ifndef EVENT_H
#define EVENT_H

#include <QWidget>
#include "usercurrentevent.h"
#include "userhistoryevent.h"
#include "ubutton.h"

namespace Ui {
class Event;
}

class Event : public QWidget
{
    Q_OBJECT

private:
    UserCurrentEvent *currRunEvent;
    UserCurrentEvent *currErrEvent;

    UserHistoryEvent *histoRunEvent;
    UserHistoryEvent *histoErrEvent;

    ubutton *pCurrentEventBtn;
    ubutton *pHistoryEventBtn;

    ubutton *pNextBtn;
    ubutton *pPreviousBtn;

    uint8_t m_usIndex;

private:
    void initTable();
    void initButton();
    void initLabel();
    void registRunEvent();
    void registErrorEvent();

public:
    explicit Event(QWidget *parent = nullptr);
    ~Event();

private:
    Ui::Event *ui;

private slots:

    void currentEventBtnCliked();
    void historyEventBtnCliked();

    void on_runButton_clicked();
    void on_errButton_clicked();

    void on_previousButton_clicked();
    void on_nextButton_clicked();
};

#endif // EVENT_H
