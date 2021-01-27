#ifndef KEYBUTTON_H
#define KEYBUTTON_H

#include <QPushButton>
#include <QTimerEvent>

class keyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit keyButton(QWidget *parent = 0);

    //是否按下
    bool isDown();
protected:
    void paintEvent(QPaintEvent *e);
    void timerEvent(QTimerEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    
private slots:
    void privatePressSlot();

private:
    bool pressDown;
    
};

#endif // KEYBUTTON_H
