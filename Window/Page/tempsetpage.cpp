#include <QDir>
#include <QTextStream>
#include <QTextCodec>
#include <unistd.h>
#include "tempsetpage.h"
#include "ui_tempsetpage.h"
#include "system.h"

#define ROOT_DIR "/home/TempSet/"
#define DEF_FILE  "tempSet.csv"
//#define DEF_FILE  "tempSet.csv"

#define DEF_TEXT_CODEC "gb2312"
#define DEF_SPLIT_C ","

#define LABEL_COLUMNS  6
#define LABEL_ROWS     15

#define LABEL_SIZE       140, 28
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     25
#define LABEL_LEFT_MARGIN   35
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  148, 50

#define DATA_LABEL_UP_MARGIN    15
#define DATA_LABEL_LEFT_MARGIN  20
#define DATA_LABEL_INTERVAL_H   150
#define DATA_LABEL_INTERVAL_V   40

#define TEMP_UNIT_NUM           88
#define MAX_GROW_DAY            462
#define MIN_GROW_DAY            -2

TempSetPage::TempSetPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TempSetPage)
{
    ui->setupUi(this);

    m_pDayMonitor = DataMonitor::monitorRegist(&System::getInstance()->m_sChickenGrowDay,
                                               Monitor::DataType::Int16t, MAX_GROW_DAY, MIN_GROW_DAY);
    connect(m_pDayMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(growDayChangedSlot(Monitor*)));

    initButton();
    readTempSetFile();
}

TempSetPage::~TempSetPage()
{
    delete ui;
}

void TempSetPage::initLabel()
{
    //ui->label->setText(QString::number(this->m_sDayIndex) + " 天");
}

void TempSetPage::initButton()
{
    TempUnit *pTempUint = nullptr;
    for (uint8_t i = 0, m = 0, n = 0; i < TEMP_UNIT_NUM; i++)
    {
        pTempUint = new TempUnit(ui->scrollArea);
        m_TempUnits.append(pTempUint);

        m = i / LABEL_COLUMNS;
        n = i % LABEL_COLUMNS;
        pTempUint->setGeometry(DATA_LABEL_LEFT_MARGIN + n * DATA_LABEL_INTERVAL_H,
                                  DATA_LABEL_UP_MARGIN + m * DATA_LABEL_INTERVAL_V,
                                  DATA_LABEL_SIZE);
        connect(pTempUint, SIGNAL(tempChanged(TempUnit *)), this, SLOT(tempChangedSlot(TempUnit *)));
    }
    //m_TempUnits[2]->setCurrentDay(true);
    m_sPreSetDay = MIN_GROW_DAY;

    connect(System::getInstance(), SIGNAL(systemTimeChanged()), this, SLOT(systemTimeChangedSlot()));
}

void TempSetPage::readTempSetFile()
{
    int start = 0;
    int end = 0;
    QByteArray dataArray;
    QString strTempSet;

    QDir path(ROOT_DIR);
    QString filePath = ROOT_DIR;

    if(!path.exists()){path.mkpath(ROOT_DIR);}
    if(!filePath.endsWith("/") )
    {
        filePath.append("/");
    }
    m_TempSetFile.setFileName(filePath.append(DEF_FILE));
    if(!m_TempSetFile.exists())
    {
        m_TempSetFile.open(QIODevice::ReadWrite);
        if(m_TempSetFile.isOpen())
        {
            for(int i = 0; i < m_TempUnits.count(); i++)
            {
                strTempSet = QString::number(i) + DEF_SPLIT_C + QString::number(m_TempUnits[i]->m_usTemp) + "\n";
                dataArray = QTextCodec::codecForName(DEF_TEXT_CODEC)->fromUnicode(strTempSet);

                m_TempSetFile.write(dataArray);
            }
            //strTempSet = QString::number(System::getInstance()->m_sChickenGrowDay)+ "\n";
            //dataArray = QTextCodec::codecForName(DEF_TEXT_CODEC)->fromUnicode(strTempSet);

            m_TempSetFile.flush();
            fsync(m_TempSetFile.handle());
            m_TempSetFile.close();
        }
    }
    else
    {
        if(m_TempSetFile.open(QIODevice::ReadWrite))
        {
            QTextStream curveDate(&m_TempSetFile);
            QString line;

            curveDate.setCodec(QString(DEF_TEXT_CODEC).toUtf8());
            line = curveDate.readLine();
            m_TempSetPoint<< line;
            while(!curveDate.atEnd())
            {
                m_TempSetPoint<<curveDate.readLine();
            }
        }
        if(!m_TempSetPoint.isEmpty())
        {
            QString line;
            QStringList lines;

            end = m_TempSetPoint.size()-1;
            for(int i = start; i < end; i++)
            {
                 line = m_TempSetPoint[i];
                 lines = line.split(DEF_SPLIT_C);

                 if(lines.count() > 1)
                 {
                     QPoint p(lines[0].toInt(), lines[1].toInt());
                     m_TempPoint.append(p);
                 }
            }
        }
        for(int i = 0; i < m_TempPoint.count(); i++)
        {
            if(i < m_TempUnits.count())
            {
                m_TempUnits[i]->m_usTemp = uint16_t(m_TempPoint[i].ry());
            }
        }
        //System::getInstance()->m_sChickenGrowDay = int16_t(m_TempSetPoint.last().toInt());
        m_TempSetFile.close();
    }

    m_sCurrentDay = System::getInstance()->m_sChickenGrowDay;
    if(MIN_GROW_DAY <= m_sCurrentDay && m_sCurrentDay < TEMP_UNIT_NUM + MIN_GROW_DAY)
    {
        m_TempUnits[m_sCurrentDay-MIN_GROW_DAY]->setCurrentDay(true);
    }
    else if(m_sCurrentDay >= TEMP_UNIT_NUM + MIN_GROW_DAY)
    {
        m_TempUnits[TEMP_UNIT_NUM - 1]->setCurrentDay(true);
    }
    m_sPreSetDay = m_sCurrentDay;
}

void TempSetPage::growDayChangedSlot(Monitor* pMonitor)
{
    QByteArray dataArray;
    QString strTempSet;

    m_sCurrentDay = int16_t(pMonitor->getCurVal());
    if(MIN_GROW_DAY <= m_sCurrentDay && m_sCurrentDay < TEMP_UNIT_NUM + MIN_GROW_DAY - 1)
    {
        m_TempUnits[m_sCurrentDay-MIN_GROW_DAY]->setCurrentDay(true);

        if(m_sPreSetDay != m_sCurrentDay)
        {
            if(m_sPreSetDay >= TEMP_UNIT_NUM + MIN_GROW_DAY)
            {
                m_TempUnits[TEMP_UNIT_NUM-1]->setCurrentDay(false);
            }
            else
            {
                m_TempUnits[m_sPreSetDay-MIN_GROW_DAY]->setCurrentDay(false);
            }
        }
    }
    else if(m_sCurrentDay >= TEMP_UNIT_NUM + MIN_GROW_DAY - 1)
    {
        m_TempUnits[TEMP_UNIT_NUM - 1]->setCurrentDay(true);
        if(m_sPreSetDay != m_sCurrentDay)
        {
            if(m_sPreSetDay < TEMP_UNIT_NUM + MIN_GROW_DAY -1)
            {
                m_TempUnits[m_sPreSetDay- MIN_GROW_DAY]->setCurrentDay(false);
            }
        }
    }
    m_sPreSetDay = m_sCurrentDay;
}

void TempSetPage::systemTimeChangedSlot()
{
    System *pSystem = System::getInstance();
    if(pSystem == nullptr){return;}

    if(pSystem->m_eSystemModeCmd == System::SystemMode::MODE_AUTO)
    {
        if(MIN_GROW_DAY <= m_sCurrentDay  && m_sCurrentDay < TEMP_UNIT_NUM + MIN_GROW_DAY)
        {
            pSystem->m_usTempSet = m_TempUnits[m_sCurrentDay-MIN_GROW_DAY]->m_usTemp;
        }
        else if(m_sCurrentDay >= TEMP_UNIT_NUM + MIN_GROW_DAY)
        {
            pSystem->m_usTempSet = m_TempUnits[TEMP_UNIT_NUM - 1]->m_usTemp;
        }
    }
}

void TempSetPage::tempChangedSlot(TempUnit*)
{
    QByteArray dataArray;
    QString strTempSet;

    if(m_TempSetFile.isOpen())
    {
        m_TempSetFile.close();
    }
    if(m_TempSetFile.open(QIODevice::ReadWrite))
    {
        for(int i = 0; i < m_TempUnits.count(); i++)
        {
            strTempSet = QString::number(i) + DEF_SPLIT_C + QString::number(m_TempUnits[i]->m_usTemp) + "\n";
            dataArray = QTextCodec::codecForName(DEF_TEXT_CODEC)->fromUnicode(strTempSet);

            m_TempSetFile.write(dataArray);
        }
        strTempSet = QString::number(0) + DEF_SPLIT_C + QString::number(0) + "\n";
        dataArray = QTextCodec::codecForName(DEF_TEXT_CODEC)->fromUnicode(strTempSet);
        m_TempSetFile.write(dataArray);

        m_TempSetFile.flush();
        fsync(m_TempSetFile.handle());
        m_TempSetFile.close();
    }
    System::getInstance()->m_uiOffLogCount = 0;
}
