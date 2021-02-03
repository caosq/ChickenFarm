#include "modularchiller.h"
#include "ui_modularchiller.h"
#include <QMessageBox>
#include "messagebox.h"

#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"

#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

#define MB_PIX2 ":UI/baseFile/mbPix2.png"

#define MODULAR_NUM_IN_CHILLER  2

#define MODULAR_UP_MARGIN     30
#define MODULAR_LEFT_MARGIN   15


#define LABEL_COLUMNS  2
#define LABEL_ROWS     3

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   80
#define LABEL_INTERVAL_H    260
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  110, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  210
#define DATA_LABEL_INTERVAL_H   300
#define DATA_LABEL_INTERVAL_V   35

uint8_t ModularChiller::m_usModularChillerCount = 0;

ModularChiller::ModularChiller(QWidget *parent) :
    Device(parent),
    ui(new Ui::ModularChiller)
{
    ui->setupUi(this);

   // m_psModular1 = new Modular(ui->frame_4);
   // m_psModular2 = new Modular(ui->frame_5);

    ModularChiller::m_usModularChillerCount++;
    this->m_usDeviceIndex = m_usModularChillerCount;

    Modular *psModular = nullptr;
    for( uint8_t i = 0; i < MODULAR_NUM_IN_CHILLER; i++)
    {
        psModular = new Modular(ui->frame);
        psModular->setGeometry(MODULAR_LEFT_MARGIN + i * psModular->width(), ui->frame_2->height(),
                               psModular->width(), psModular->height());
        m_Modulars.append(psModular);
    }
    initLabel();
}

void ModularChiller::initLabel()
{
    TextLabel *pLabel = nullptr;

    for(uint8_t n = 0; n < LABEL_ROWS; n++)
    {
        for(uint8_t m = 0; m < LABEL_COLUMNS; m++)
        {
            pLabel = new TextLabel(ui->frame_2);
            pLabel->setGeometry( LABEL_LEFT_MARGIN + m * LABEL_INTERVAL_H,
                                 LABEL_UP_MARGIN + n * LABEL_INTERVAL_V,
                                 LABEL_SIZE);
            m_Labels.append(pLabel);
        }
    }
    m_Labels[0]->setText(tr("模块状态"), LABEL_FONT_SIZE);
    m_Labels[1]->setText(tr("进水温度"), LABEL_FONT_SIZE);
    m_Labels[2]->setText(tr("出水温度"), LABEL_FONT_SIZE);
    m_Labels[3]->setText(tr("运行标志"), LABEL_FONT_SIZE);
    m_Labels[4]->setText(tr("水流开关"), LABEL_FONT_SIZE);

    ui->label->setText(QString::number(this->m_usDeviceIndex) + "# 机组");
    //ui->label->setGeometry(128, 15, ui->label->width(), ui->label->height());
}


ModularChiller::~ModularChiller()
{
    delete ui;
}
