#include "modularchillerpage.h"
#include "ui_modularchillerpage.h"

#define LABEL_COLUMNS_1  1
#define LABEL_ROWS_1     5

#define LABEL_COLUMNS_2  1
#define LABEL_ROWS_2     2

#define LABEL_SIZE       140, 25
#define LABEL_FONT_SIZE  14

#define LABEL_UP_MARGIN     30
#define LABEL_LEFT_MARGIN   30
#define LABEL_INTERVAL_H    300
#define LABEL_INTERVAL_V    35

#define DATA_LABEL_SIZE  110, 25

#define DATA_LABEL_UP_MARGIN    30
#define DATA_LABEL_LEFT_MARGIN  210
#define DATA_LABEL_INTERVAL_H   300
#define DATA_LABEL_INTERVAL_V   35

#define MODULAR_CHILLER_NUM   2


ModularChillerPage::ModularChillerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModularChillerPage)
{
    ui->setupUi(this);

    initDevice();
    initLabel();
    initButton();
}

ModularChillerPage::~ModularChillerPage()
{
    delete ui;
}

void ModularChillerPage::initDevice()
{
    ModularChiller* m_psModularAir = nullptr;
    for(uint8_t n = 0; n < MODULAR_CHILLER_NUM; n++)
    {
        m_psModularAir = new ModularChiller();
        m_ModularChillers.append(m_psModularAir);
       // ui->ModularChillerStackedWidget->insertWidget(n, m_psModularAir);
    }
}
