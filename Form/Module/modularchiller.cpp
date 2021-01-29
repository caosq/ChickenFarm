#include "modularchiller.h"
#include "ui_modularchiller.h"
#include <QMessageBox>
#include "messagebox.h"

#define AB_PRESS_PIX ":UI/baseFile/abPress.png"
#define AB_RELEASE_PIX ":UI/baseFile/abRelease.png"

#define SBUTTON_STATE0 ":UI/baseFile/sbLeft.png"
#define SBUTTON_STATE1 ":UI/baseFile/sbRight.png"

#define MB_PIX2 ":UI/baseFile/mbPix2.png"

ModularChiller::ModularChiller(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularChiller)
{
    ui->setupUi(this);

   // m_psModular1 = new Modular(ui->frame_4);
   // m_psModular2 = new Modular(ui->frame_5);

    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::press,AB_PRESS_PIX);
    AnalogValButton::forefather()->setFatherPixmap(AnalogValButton::release,AB_RELEASE_PIX);

    m_pButton = new AnalogValButton(ui->frame_2);
    m_pButton->setGeometry(0,0,100,30);

    // m_pLabel = new DataLabel(ui->frame_2);

     //m_pLabel->setText("hahha");


    StateButton::forefather()->setFatherPixmap(eButtonState::State0,SBUTTON_STATE0);
    StateButton::forefather()->setFatherPixmap(eButtonState::State1,SBUTTON_STATE1);
    StateButton::forefather()->setFatherFontSize(12);

    debugMod = new StateButton(ui->frame_2);
    debugMod->setGeometry(200,0,100,30);
    debugMod->setStateText(eButtonState::State0,tr("屏蔽"));
    debugMod->setStateText(eButtonState::State1,tr("开启"));
    debugMod->setDeafultState(eButtonState::State0);


    ModeButton::forefather()->setFatherPixmap(MB_PIX2);

    waterType = new ModeButton(ui->frame_2);
    waterType->setGeometry(100,0,100,30);
    waterType->setItem(0,tr("无  "));
    waterType->setItem(1,tr("类型一"));
    waterType->setItem(2,tr("类型二"));
    waterType->setItem(3,tr("类型三"));
    waterType->setItem(4,tr("类型四"));
    waterType->setDefaultValue(0);


}

ModularChiller::~ModularChiller()
{
    delete ui;
}

void ModularChiller::on_pushButton_clicked()
{
    static messageBox universalMsg;
    universalMsg.setInformativeText("hello");
    universalMsg.show();
}
