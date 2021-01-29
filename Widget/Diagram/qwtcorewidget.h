#ifndef QWTCOREWIDGET_H
#define QWTCOREWIDGET_H

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <qwt_plot_textlabel.h>
#include <qwt_plot_legenditem.h>
#include <qwt_scale_draw.h>

#include "curvetracker.h"

#include <QScrollBar>
#include <QPushButton>
#include <QTime>
#include <QLabel>


class QwtScrollBar:public QScrollBar
{
    Q_OBJECT
public:
    QwtScrollBar( QWidget *parent = NULL );

    bool sliderIsMove();

protected:
    bool eventFilter(QObject *obj, QEvent *event);


private:
    bool barMove;
    bool timeState;

signals:

public slots:

};

class ScrollBarWidget:public QWidget
{
    Q_OBJECT
public:
    ScrollBarWidget( QWidget *parent = NULL , int width = 850,int height = 22);

    static void getIcon()
    {
        if( NULL == previouIcon )
        {
            previouIcon = new QIcon;
            previouIcon->addPixmap(QPixmap(":/images/baseFile/sc_bar_former.png"));

            nextIcon = new QIcon;
            nextIcon->addPixmap(QPixmap(":/images/baseFile/sc_bar_next.png"));
        }
    }

    QwtScrollBar *BarHandle();
    void setBarMaxmum(int max);
    void setBarMinimum(int min);

    void setBarVal(int val);

    void setBarPageStep(int step);
    void setBarStep(int step);

    void upDateBarGeometry();

    void MaxmumMountUp();
    void ReloadRange();

    void gotoMax();

protected:


private:
    QwtScrollBar *my_bar;
    QPushButton  *nextPage;
    QPushButton  *previouPage;

    static QIcon *previouIcon;
    static QIcon *nextIcon;

    QPoint mousePosition;

    int max_val;

    int press_val;
    int release_val;

signals:
    void valChanged(int val);

public slots:
    void barSLot(int val);
    void sliderReleaseSLot();
    void sliderPressSlot();
    void nextPageSlot();
    void previouPageSLot();

};

class LegendItem: public QwtPlotLegendItem
{
public:
    LegendItem();
};

class QwtCurveCore : public QwtPlot
{
    Q_OBJECT
public:
    explicit QwtCurveCore(QWidget *parent = 0);


public:
    bool AddCurve(unsigned int index = 0);

    QwtPlotCanvas *CanvasHandle();
    QwtPlotGrid *GridHandle();
    QwtPlotCurve *CurveHandle(unsigned int index);

    void setCurrentTime(QString str = "");
    void setHistoryTime(QString str = "");
    void setDateAttribute(unsigned int dot = 0,QString unit = "");
    QwtPlotTextLabel *historyLabelHandle();
    QwtText *historyText();

    QColor getCurveColor(int index);
    int CurveDot();
    QString CurveUnit();
    void clearItemData();
    void clearAllCurveData();
    /************************************************************
    *由于历史控件只在show时才会刷新(为了缩短启动时间)
    *会导致没加载曲线的情况下调用setItemName失败
    *(结果就是设置了条目名称但不能显示)
    *因为曲线的顺序决定了曲线的颜色，所以可以根据序号获取曲线颜色
    *(另一种曲线获取方式如果检测到对应序号的曲线没有加载会返回失败)
    *这里提供一个强制获取的接口
    ************************************************************/
    QColor forcibleGetCurveColor(int index);
private:
    void InitMember();
    void SetLegendFormatDate(unsigned int index);

private:
    QwtPlotCanvas *canvas;
    QwtPlotGrid *grid;
    QVector<QwtPlotCurve *> curve_array;

    LegendItem  *my_legend;

    QwtText *history_time;
    QwtText *current_time;

    QwtPlotTextLabel *historyLabel;
    QwtPlotTextLabel *currentLabel;

    unsigned int curve_dot;
    QString curve_unit;
    QVector<QColor> colorList;
protected:

signals:

public slots:
    //void showItem(const QVariant &itemInfo, bool on);

};

class TimeScaleDraw:public QwtScaleDraw
{
public:
    TimeScaleDraw(const QTime &base)
        : baseTime(base)
    {
    }

    virtual QwtText label(double v) const
    {
        QTime updateTime = baseTime.addSecs(static_cast<int>(v));

        QFont font("Bitstream Charter", 15);
        font.setBold(true);

        QwtText text = updateTime.toString();
        text.setFont(font);
        return text;
    }

private:
    QTime baseTime;
};

class QwtNameItem: public QWidget
{
    Q_OBJECT
public:
    QwtNameItem(QWidget *parent = 0,int num = 0);

    void setItemColor(QColor color);
    void setItemName(QString str);

    void m_setGeometry(int x, int y, int w, int h);

private:
    int minWidth;
    int minHeight;

    QLabel  *colorBlock;
    QLabel  *curveName;
    int index;
};

class QwtCoreWidget: public QWidget
{
    Q_OBJECT
public:
    QwtCoreWidget(QWidget *parent = 0);

    void setXAxisScale(double min,double max,double step = 0);
    void setYAxisScale(double min,double max,double step = 0);
    void getXAxisScale(double *min,double *max,double *step = 0);
    void getYAxisScale(double *min,double *max,double *step = 0);

    void natureReloadXAxisScale();
    void coercedReloadXAxisScale();

    void LoadTimeScale(QDateTime start_time);
    void UpdateTimeScale(unsigned int SampleStep = 10);
    void setJumpStep(int JumpStep = 120);

    bool AddCurve(unsigned int index = 0);
    bool hideCurve(unsigned int index);
    bool showCurve(unsigned int index);
    bool curveIsVisible(unsigned int index);

    void setItemGeometry(int index,int x, int y, int w, int h);
    bool setItemName(int index,QString str);
    void itemNameShowOrHide(int index,bool state);

    void setCurrentTimeMark(QDateTime c_date);
    void setCurrentTimeMark(QString str);
    void setHistoryTimeMark(QString str);

    void setSamples(unsigned int index,const QVector<QPointF> points);
    void setCurveAttribute(unsigned int dot = 0,QString unit = "");

    QwtCurveCore *getPlotHandle();
    void m_setGeometry(int x, int y, int w, int h);

    void setTrackerDate(QDateTime date,bool soigne = false);
    double CurrentScaleVal();
    double xScaleConverssionMin();

    QDateTime currentDateTime();
    QString currentTimeStr();

    void coerceUpdateCDT(QDateTime temp);/*CDT:currentDateTime*/

    bool IsNewDay();

    void clearCurrentTimeMark();
    void clearHistoryTimeMark();

    unsigned int getCoreAstrictNum();
    QString getBaseSavePath();
    QString ConfigFileName();

    int curveDot();
    QString curveUnit();
    int JumpStep();
    ScrollBarWidget* scrollBarHandle();

    void forcibleMakeItemList(int index);

private:
    void updateXScaleByBar(double min,double max);
    void setScrollBarAttribute();
    void conversionToZeroTime(QDateTime d_time);
    void setBarSignalStep(int step);
    void setBarPageStep(int step);


signals:
    void xScrolChange(double min,double max);
protected:
    bool eventFilter(QObject *, QEvent *);

public slots:
    void BarSlot(int val);

private:
    QWidget *parentPointer;

    QVector<QwtNameItem *> nameItemList;

    CurveTracker *my_tracker;
    QwtCurveCore *my_curve;
    ScrollBarWidget *my_scrollBar;

    TimeScaleDraw   *time_scale;

    QString currentTime;

    QTime zeroTime;
    double current_scale_val;

    QDateTime currentDate;
    QDateTime baseDate;

    bool updateXScale;

    double X_ScaleConverssionMin;
    double X_ScaleConverssionMax;

    double X_ScaleMinBase;
    double X_ScaleMaxBase;

    double X_ScaleMin;
    double X_ScaleMax;
    double X_ScaleStep;

    double X_ScaleJumpStep;

    double Y_ScaleMin;
    double Y_ScaleMax;
    double Y_ScaleStep;
};

#endif // QWTCOREWIDGET_H
