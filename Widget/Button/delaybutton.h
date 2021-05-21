#ifndef DELAYBUTTON_H
#define DELAYBUTTON_H

#include <QWidget>
#include "statebutton.h"
#include "messagebox.h"

class DelayButton : public StateButton
{
    Q_OBJECT
public:
    explicit DelayButton(QWidget *parent = nullptr);

public:
     void  setDelayTime(char time);
     void chooseMessage(char id = 0);

private:
     void initMessage();

signals:

private slots:
    void delayClicked(int val);
    void delayTimeSlot();
    void messageBoxShow();

private:
    QTimer *delayTimer;
    char delayTime;
    messageBox *temp;
    messageBox *confirmationBox;
    static QVector<messageBox *> msg;
};

#endif // DELAYBUTTON_H
