#ifndef CURVETRACKER_H
#define CURVETRACKER_H

#include <qwt_plot_picker.h>
#include <qwt_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_text.h>
#include <qwt_plot_textlabel.h>

#include <QDateTime>
#include <QDebug>


class QwtPlotCurve;

class CurveTracker: public QwtPlotPicker
{
public:
    CurveTracker(QWidget *canvas);


    typedef struct{
        double date;
        bool lineIsNull;
    }x_date;

    void setCurveAttribute(unsigned int dot = 0,QString unit = "");
    void setCurveDate(QDateTime qdate,bool soigne = false);

    void setXText(QwtText *addr);
    void setXTextLabel(QwtPlotTextLabel *addr);

    void setTimeStep(unsigned int step = 10);

protected:
    virtual QwtText trackerTextF( const QPointF & ) const;
    //virtual QRect trackerRect( const QFont & ) const;

private:
    x_date curveInfoAt( QwtPlotCurve *, const QPointF & ) const;
    QLineF curveLineAt( const QwtPlotCurve *, double x ) const;

    bool TimeMode;

    int curve_dot;
    QString curve_unit;
    QDateTime current_date;

    QString yearMonDay;
    QTime baseTime;
    unsigned int time_step;

    QwtText *x_val;
    QwtPlotTextLabel *x_label;

};

#endif // CURVETRACKER_H
