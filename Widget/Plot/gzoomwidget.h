#ifndef GZOOMWIDGET_H
#define GZOOMWIDGET_H

#include <QWidget>
#include "gtoolbutton.h"

class gZoomWidget : public QWidget
{
    Q_OBJECT
public:
    //放大控件
    explicit gZoomWidget(QWidget *parent = 0);
    
    QSize sizeHint();
private:
    gtoolButton *_btnZoom;
    gtoolButton *_clearZoom;

signals:
    void zoomToggled(bool);
    void clearZoom();
    
public slots:
    
};

#endif // GZOOMWIDGET_H
