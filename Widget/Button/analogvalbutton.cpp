#include "analogvalbutton.h"
#include <qmath.h>
#include <QPainter>
#include "akeyboard.h"
#include "textcontrol.h"

#define MAX_DEC_POINT 3

#define SP_X 3
#define SP_Y 3

AnalogValButton *AnalogValButton::father = nullptr;
QFont AnalogValButton::fatherFont = textControl::instance()->font(textControl::size20);
QPixmap *AnalogValButton::_pix0 = nullptr;
QPixmap *AnalogValButton::_pix1 = nullptr;


AnalogValButton::AnalogValButton(QWidget *parent) :
    Button(parent),m_pMonitor(nullptr),m_pMaxMonitor(nullptr),m_pMinMonitor(nullptr)
{
    m_strUnit = "";
    m_ucDecPoint = 0;
    m_eDataType = Uint16t;

    defValState = true;
    enableValMarker = true;
    save = true;
    automaticAttack = true;

    m_iMaxVal = 9999;
    m_iMinVal = -9999;
    m_iCurrentVal = 0;
    m_strCurrentText = "0";

    defVal = 0;

    m_pPressImg = nullptr;
    m_pPressImg = nullptr;

    m_alignment = Qt::AlignCenter;
    m_iMargin = 0;

    setFont(fatherFont);
    if(father)
    {
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }
    connect(this,SIGNAL(clicked()),this,SLOT(clickedSlot()));
}

AnalogValButton *AnalogValButton::forefather()
{
    if(father == nullptr)
    {
        father = new AnalogValButton;
    }
    return father;
}

bool AnalogValButton::setMonitorData(void* pvVal, eDataType emDataType)
{
    m_pMonitor = DataMonitor::monitorRegist(pvVal, emDataType);

    if(m_pMonitor != nullptr)
    {
        connect(m_pMonitor, SIGNAL(valChange(uint)), this, SLOT(setValue(uint)));
        setValue(m_pMonitor->getCurVal());
        return true;
    }
    return false;
}

void AnalogValButton::setDataParameter(eDataType emDataType, QString strUnit, uint8_t ucDecPoint,
                                       int32_t iMaxVal, int32_t iMinVal, int32_t iDefaultVal)
{
    m_strUnit     = strUnit;
    m_iDefaultVal = iDefaultVal;
    m_iMaxVal     = iMaxVal;
    m_iMinVal     = iMinVal;
    m_eDataType   = emDataType;

    setDecPoint(ucDecPoint);

    if(m_pMonitor)
    {
        m_pMonitor->setValRange(iMaxVal, iMinVal);
        m_pMonitor->setValType(emDataType);
    }
}

void AnalogValButton::setDecPoint(uint8_t ucDecPoint)
{
    if(ucDecPoint > MAX_DEC_POINT)
    {
        return;
    }
    m_ucDecPoint = ucDecPoint;
}

void AnalogValButton::setDataType(eDataType emDataType)
{
    m_eDataType = emDataType;
    if(m_pMonitor)
    {
        m_pMonitor->setValType(emDataType);
    }
    else
    {

    }
}

bool AnalogValButton::setMaxValMonitor(void* pvVal, eDataType emDataType)
{
    if(m_pMaxMonitor == nullptr)
    {
        m_pMaxMonitor = DataMonitor::monitorRegist(pvVal, emDataType);
        connect(m_pMaxMonitor, SIGNAL(valChange(uint)),this,SLOT(setMaxValue(uint)));

        setMaxValue(m_pMaxMonitor->getCurVal());
        return true;
    }
    else
    {
        return false;
    }
}

bool AnalogValButton::setMinValMonitor(void* pvVal, eDataType emDataType)
{
    if(m_pMinMonitor == nullptr)
    {
        m_pMinMonitor = DataMonitor::monitorRegist(pvVal, emDataType);
        connect(m_pMinMonitor, SIGNAL(valChange(uint)),this,SLOT(setMinValue(uint)));

        setMinValue(m_pMinMonitor->getCurVal());
        return true;
    }
    else
    {
        return false;
    }
}

void AnalogValButton::setFatherFontSize(int size)
{
    if(this == father)
    {
        if( fatherFont.pointSize() != size )
        {
            fatherFont.setPointSize(size);
            emit fontChange();
        }
    }
}

void AnalogValButton::setFatherPixmap(STATE state, QString filePath)
{
    if( father == this )
    {
        if( press == state )
        {
            if( _pix0 )
            {
                _pix0->load(filePath);
            }
            else
            {
                _pix0 = new QPixmap(filePath);
            }
        }
        else if(release == state)
        {
            if( _pix1 )
            {
                _pix1->load(filePath);
            }
            else
            {
                _pix1 = new QPixmap(filePath);
            }
        }
    }
}


/*void AnalogValButton::setUnit(QString strUnit)
{
    QString str = text();
    if("" == m_strUnit)
    {
        QButton::setText(str.append(strUnit));
    }
    else
    {
        QButton::setText(str.replace(m_strUnit, strUnit));
    }
    m_strUnit = strUnit;
}*/

void AnalogValButton::setValue(uint32_t val)
{

    switch(m_eDataType)
    {
    case eDataType::Uint8t:
    case eDataType::Uint16t:
    case eDataType::Uint32t:
    {
        uint32_t tempval = uint32_t(defVal);
        m_iCurrentVal = val;
        defValState = (tempval == val) ? true:false;
        m_strCurrentText = QString::number( val/qPow(10, m_ucDecPoint), '.', m_ucDecPoint).append(m_strUnit );
    }
        break;
    case eDataType::Int8t:
    {
        int8_t tempval = val;
        m_iCurrentVal = tempval;
        defValState = (defVal == tempval) ? true:false;
        m_strCurrentText = QString::number( tempval/qPow(10, m_ucDecPoint), '.', m_ucDecPoint).append(m_strUnit );
    }
        break;
    case eDataType::Int16t:
    {
        int16_t tempval = val;
        m_iCurrentVal = tempval;
        defValState = (defVal == tempval) ? true:false;
        m_strCurrentText = QString::number( tempval/qPow(10, m_ucDecPoint), '.', m_ucDecPoint).append(m_strUnit );
    }
        break;
    case eDataType::Int32t:
    {
        int32_t tempval = val;
        m_iCurrentVal = tempval;
        defValState = (defVal == tempval) ? true:false;
        m_strCurrentText = QString::number( tempval/qPow(10, m_ucDecPoint), '.', m_ucDecPoint).append(m_strUnit );
    }
        break;
    default:
        break;
    }
    if(m_pMonitor)
    {
        m_pMonitor->setValue(val);
    }
    QPushButton::setText(m_strCurrentText);
    update();
}

void AnalogValButton::setMaxValue(uint32_t val)
{

    switch(m_eDataType)
    {
    case eDataType::Uint8t:
    case eDataType::Uint16t:
    case eDataType::Uint32t:
    {
        m_iMaxVal = val;
    }
        break;
    case eDataType::Int8t:
    {
        int8_t tempval = val;
        m_iMaxVal = tempval;
    }
        break;
    case eDataType::Int16t:
    {
        int16_t tempval = val;
        m_iMaxVal = tempval;
    }
        break;
    case eDataType::Int32t:
    {
        int32_t tempval = val;
        m_iMaxVal = tempval;
    }
        break;
    default:
        break;
    }
    if(m_pMonitor)
    {
        m_pMonitor->setValRange(m_iMaxVal, m_iMinVal);
    }
}

void AnalogValButton::setMinValue(uint32_t val)
{

    switch(m_eDataType)
    {
    case eDataType::Uint8t:
    case eDataType::Uint16t:
    case eDataType::Uint32t:
    {
        m_iMinVal = val;
    }
        break;
    case eDataType::Int8t:
    {
        int8_t tempval = val;
        m_iMinVal = tempval;
    }
        break;
    case eDataType::Int16t:
    {
        int16_t tempval = val;
        m_iMinVal = tempval;
    }
        break;
    case eDataType::Int32t:
    {
        int32_t tempval = val;
        m_iMinVal = tempval;
    }
        break;
    default:
        break;
    }
    if(m_pMonitor)
    {
        m_pMonitor->setValRange(m_iMaxVal, m_iMinVal);
    }
}

void AnalogValButton::setText(const QString text, int size)
{
    m_strCurrentText = text;
    QPushButton::setText(m_strCurrentText);

    setTextSize(size);
    update();
}

void AnalogValButton::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void AnalogValButton::setTextColor(QPalette::ColorGroup group, QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(group,QPalette::ButtonText,QBrush(color));
    setPalette(palette);
}

void AnalogValButton::setPixmap(STATE state, QString filePath)
{
    if(press == state)
    {
        if(m_pPressImg)
        {
            m_pPressImg->load(filePath);
        }
        else
        {
            m_pPressImg = new QPixmap(filePath);
        }
    }
    else if(release == state)
    {
        if(m_pRelaseImg)
        {
            m_pRelaseImg->load(filePath);
        }
        else
        {
            m_pRelaseImg = new QPixmap(filePath);
        }
    }
}

void AnalogValButton::fontSlot()
{
    setFont(fatherFont);
}

void AnalogValButton::setClicked()
{
    clickedSlot();
}

void AnalogValButton::clickedSlot()
{
    int tempVal = aKeyBoard::getValue(m_iCurrentVal, m_iMaxVal, m_iMinVal, m_ucDecPoint);
    setValue(tempVal);
}

void AnalogValButton::paintEvent(QPaintEvent *e)
{
    if(m_pPressImg && m_pRelaseImg)
    {
        QPainter painter(this);
        if( !isEnabled() )
        {
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }else{
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        if( this->isDown() )
        {
            painter.drawPixmap(1, 1, this->width(), this->height(), *m_pPressImg);
            painter.drawText(m_iMargin + 1, 1, this->width(),this->height(), m_alignment, m_strCurrentText);
        }else{
            painter.drawPixmap(0, 0, this->width(), this->height(), *m_pRelaseImg);
            painter.drawText(m_iMargin, 0, this->width(),this->height(), m_alignment, m_strCurrentText);
        }

    }
    else if(m_pPressImg && m_pRelaseImg )
    {
        QPainter painter(this);
        if( !isEnabled() )
        {
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }
        else
        {
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        if( this->isDown() )
        {
            painter.drawPixmap(1, 1, this->width(), this->height(), *m_pPressImg);
            painter.drawText(m_iMargin + 1, 1, this->width(),this->height(), m_alignment, m_strCurrentText);

        }
        else
        {
            painter.drawPixmap(0, 0, this->width(), this->height(), *m_pRelaseImg);
            painter.drawText(m_iMargin,0,this->width(), this->height(), m_alignment, m_strCurrentText);
        }

    }
    else
    {
        QPushButton::paintEvent(e);
        QPainter painter(this);
        if( !isEnabled() )
        {
            painter.setPen(this->palette().color(QPalette::Inactive,QPalette::ButtonText));
        }
        else
        {
            painter.setPen(this->palette().color(QPalette::Active,QPalette::ButtonText));
        }
        painter.drawText(m_iMargin, 0, this->width(), this->height(), m_alignment, m_strCurrentText);
    }

      this->setStyleSheet("background-color: rgba(191, 64, 64, 0);border:none");

//    if( !defValState && enableValMarker )
//    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::green);
        painter.drawEllipse(SP_X, SP_Y, 5, 5);
//    }
}

void AnalogValButton::setTextAlignment(Qt::Alignment flag)
{
    m_alignment = flag;
}

void AnalogValButton::setTextMargin(int margin)
{
    m_iMargin = margin;
}

void AnalogValButton::setValueRange(int max, int min)
{
    m_iMaxVal = max;
    m_iMinVal = min;

    if(m_pMonitor)
    {
        m_pMonitor->setValRange(m_iMaxVal, m_iMinVal);
    }
}

/*void AnalogValButton::disableSave(bool state)
{
    save = state;
}*/

void AnalogValButton::setDefaultValue(int defValue)
{
    defVal = defValue;
    defValState = (m_iCurrentVal == defVal) ? true:false;
    update();
}
