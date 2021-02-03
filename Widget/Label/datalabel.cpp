#include "datalabel.h"
#include <qmath.h>

#define MAX_DEC_POINT 3
#define HS 4
#define WS 4
#define ROUND_RADIUS 4

DataLabel *DataLabel::father = nullptr;
QFont *DataLabel::fatherFont = nullptr;

DataLabel::DataLabel(QWidget *parent, eLabelType mLabelType, Monitor::DataType meDataType):
    QLabel(parent)
{
    m_eLabelType = mLabelType;
    m_eDataType  = meDataType;

    m_strUnit = "";
    m_ucDecPoint = 0;

    m_eLabelStyle = eLabelStyle::Rectangle;
    m_xTextColorLock = false;

    if(father && fatherFont)
    {
        setFont(*fatherFont);
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }
    else
    {
        setFont(textControl::instance()->font(textControl::size18));
    }

    setPalette(*(textControl::instance()->palette()));
    setAlignment(Qt::AlignRight);
}

DataLabel *DataLabel::forefather()
{
    if(father == nullptr)
    {
        father = new DataLabel();
    }
    return father;
}

bool DataLabel::setMonitorData(void* pvVal, Monitor::DataType emDataType)
{
    m_pMonitor = DataMonitor::monitorRegist(pvVal, emDataType);

    if(m_pMonitor != nullptr)
    {
        connect(m_pMonitor,SIGNAL(valChange(uint)),this,SLOT(setValue(uint)));
        setValue(m_pMonitor->getCurVal());
        return true;
    }
    return false;
}

void DataLabel::setDataParameter(QString unit, uint8_t dot, Monitor::DataType emDataType)
{
    m_strUnit = unit;
    m_eDataType = emDataType;
    setDecPoint(dot);
}

void DataLabel::setDataType(Monitor::DataType emDataType)
{
    m_eDataType = emDataType;
    if(m_pMonitor)
    {
        m_pMonitor->setValType(emDataType);
    }
    else
    {
         setValue(0);
    }
}

void DataLabel::setUnit(QString strUnit)
{
    if(m_eLabelType != Data )
    {
        return;
    }
    else
    {
        QString str = text();
        if("" == m_strUnit)
        {
            QLabel::setText(str.append(strUnit));
        }
        else
        {
            QLabel::setText(str.replace(m_strUnit, strUnit));
        }
        m_strUnit = strUnit;
    }
}

void DataLabel::setDecPoint(uint8_t ucDecPoint)
{
    if(ucDecPoint > MAX_DEC_POINT)
    {
        return;
    }
    else
    {
        m_ucDecPoint = ucDecPoint;
    }
}

void DataLabel::setValueMap(int val, QString str)
{
    sTextMap temp;
    if( m_TextMap.contains(val) )
    {
        temp = m_TextMap.value(val);
        temp.textStr = str;
    }
    else
    {
        temp.textStr = str;
        temp.textColor = QColor(Qt::black);
        temp.backgroundColor = QColor(Qt::white);
        temp.pixmap = nullptr;
    }
    m_TextMap.insert(val,temp);
}

void DataLabel::setValueMap(int val, QString str, QColor textColor)
{
    sTextMap temp;
    if(m_TextMap.contains(val))
    {
        temp = m_TextMap.value(val);
        temp.textColor = textColor;
        temp.textStr = str;
    }
    else
    {
        temp.textStr = str;
        temp.textColor = textColor;
        temp.backgroundColor = QColor(Qt::white);
        temp.pixmap = nullptr;
    }
    m_TextMap.insert(val,temp);
}

void DataLabel::setValueMap(int val, QColor backgroundColor)
{
    sTextMap temp;
    if( m_TextMap.contains(val) )
    {
        temp = m_TextMap.value(val);
        temp.backgroundColor = backgroundColor;
    }
    else
    {
        temp.textStr = "";
        temp.textColor = textControl::instance()->brush()->color();
        temp.backgroundColor = backgroundColor;
        temp.pixmap = nullptr;
    }
    m_TextMap.insert(val,temp);
}

void DataLabel::setValueMap(int val, QPixmap *pixmap)
{
    sTextMap temp;
    if( m_TextMap.contains(val) )
    {
        temp = m_TextMap.value(val);
        temp.pixmap = pixmap;
    }else{
        temp.textStr = "";
        temp.textColor = textControl::instance()->brush()->color();
        temp.backgroundColor = QColor(Qt::white);
        temp.pixmap = pixmap;
    }
    m_TextMap.insert(val,temp);
}

void DataLabel::lockTextColor(QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(QPalette::Active,QPalette::WindowText,QBrush(color));
    palette.setBrush(QPalette::Inactive,QPalette::WindowText,QBrush(color));
    setPalette(palette);

    m_xTextColorLock = true;
    update();
}

void DataLabel::unlockTextColor()
{
    m_xTextColorLock = false;

    if(m_eLabelType == eLabelType::Text && m_pMonitor != nullptr)
    {
        setValue(m_pMonitor->getCurVal());
    }
}

void DataLabel::showData(unsigned int val)
{
    switch(m_eDataType)
    {
    case Monitor::Uint8t:
    case Monitor::Uint16t:
    case Monitor::Uint32t:
    {
        setText(QString::number( val/uint32_t(qPow(10, m_ucDecPoint)), '.', m_ucDecPoint).append(m_strUnit) );
    }
        break;
    case Monitor::Int8t:
    {
        int8_t tempval = val;
        setText(QString::number( tempval/qPow(10, m_ucDecPoint), '.', m_ucDecPoint).append(m_strUnit) );
    }
        break;
    case Monitor::Int16t:
    {
        int16_t tempval = val;
        setText(QString::number( tempval/qPow(10, m_ucDecPoint), '.', m_ucDecPoint).append(m_strUnit) );
    }
        break;
    case Monitor::Int32t:
    {
        int32_t tempval = val;
        setText(QString::number( tempval/uint32_t(qPow(10, m_ucDecPoint)), '.', m_ucDecPoint).append(m_strUnit) );
    }
        break;
    default: break;
    }
}

void DataLabel::showText(unsigned int val)
{
    sTextMap temp;
    temp.textStr = "";
    temp.textColor = QColor(Qt::white);

    switch(m_eDataType)
    {
    case Monitor::Uint8t:
    case Monitor::Uint16t:
    case Monitor::Uint32t:
    {
        if( m_TextMap.contains(val) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int8t:
    {
        int8_t tempval = val;
        if( m_TextMap.contains(tempval) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int16t:
    {
        int16_t tempval = val;
        if( m_TextMap.contains(tempval) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int32t:
    {
        int32_t tempval = val;
        if( m_TextMap.contains(tempval) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    default:
        break;
    }

    //setTextColor(temp.textColor);
    setText(temp.textStr);
}

void DataLabel::showColor(unsigned int val)
{
    sTextMap temp;
    temp.backgroundColor = QColor(Qt::black);

    switch(m_eDataType){
    case Monitor::Uint8t:
    case Monitor::Uint16t:
    case Monitor::Uint32t:
    {
        if( m_TextMap.contains(val) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int8t:
    {
        int8_t tempval = val;
        if( m_TextMap.contains(tempval) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int16t:
    {
        int16_t tempval = val;
        if( m_TextMap.contains(tempval) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int32t:
    {
        int32_t tempval = val;
        if( m_TextMap.contains(tempval) )
        {
            temp = m_TextMap.value(val);
        }
    }
        break;
    default:
        break;
    }
    setBackGroundColor(temp.backgroundColor);
}

void DataLabel::showImage(unsigned int val)
{
    sTextMap temp;
    temp.pixmap = 0;

    switch(m_eDataType){
    case Monitor::Uint8t:
    case Monitor::Uint16t:
    case Monitor::Uint32t:{
        if( m_TextMap.contains(val) ){
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int8t:{
        int8_t tempval = val;
        if( m_TextMap.contains(tempval) ){
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int16t:{
        int16_t tempval = val;
        if( m_TextMap.contains(tempval) ){
            temp = m_TextMap.value(val);
        }
    }
        break;
    case Monitor::Int32t:{
        int32_t tempval = val;
        if( m_TextMap.contains(tempval) ){
            temp = m_TextMap.value(val);
        }
    }
        break;
    default:
        break;
    }
    if(temp.pixmap)
    {
        setPixmap(*(temp.pixmap));
    }
}

void DataLabel::setText(const QString text, int size)
{
    setTextSize(size);
    QLabel::setText(text);
}

void DataLabel::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);

    setFont(font);
}

void DataLabel::setTextSize(textControl::textSize size)
{
    setFont(textControl::instance()->font(size));
}

void DataLabel::flush()
{
    if(m_pMonitor)
    {
        setValue(m_pMonitor->getCurVal());
    }
}

void DataLabel::setFatherFontSize(int size)
{
    if(this == father)
    {
        if(fatherFont)
        {
            fatherFont->setPointSize(size);
        }
        else
        {
            fatherFont = new QFont;
            fatherFont->setPointSize(size);
        }
        emit fontChange();
    }
}

void DataLabel::setTextColor(QColor color)
{
    if( !_textColorLock ){
        QPalette    palette = this->palette();
        palette.setBrush(QPalette::Active,QPalette::WindowText,QBrush(color));
        palette.setBrush(QPalette::Inactive,QPalette::WindowText,QBrush(color));
        setPalette(palette);
    }
}

void DataLabel::setBackGroundColor(QColor color)
{
    this->setAutoFillBackground(true);
     QPalette palette = this->palette();
     palette.setBrush(QPalette::Normal, QPalette::Window, QBrush(color));
     palette.setBrush(QPalette::Inactive, QPalette::Window, QBrush(color));
     setPalette(palette);
}

void DataLabel::setBackGroundColorStyle(eLabelStyle style)
{
    m_eLabelStyle = style;
}

void DataLabel::setValue(unsigned int val)
{
    switch(m_eLabelType)
    {
    case eLabelType::Data:
    {
        showData(val);
    }
        break;
    case eLabelType::Text:
    {
        showText(val);
    }
        break;
    case eLabelType::Color:
    {
        showColor(val);
    }
        break;
    case eLabelType::Image:
    {
        showImage(val);
    }
        break;
    }
}

void DataLabel::paintEvent(QPaintEvent *e)
{
    if(m_eLabelStyle == eLabelStyle::Circular && m_eLabelType == eLabelType::Color)
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing,true);
        QColor color = palette().color(QPalette::Normal,QPalette::Window);

        qreal radius = (height() - HS * 2) / 2;
        qreal dia = radius * 2;
        qreal cx = radius + WS;
        qreal cy = radius + HS;

        QRadialGradient tb(cx,cy,radius,cx + radius / 2 ,cy - radius / 2);

        tb.setColorAt(0.0,Qt::white);
        tb.setColorAt(0.356,color);

        p.setPen(QPen(Qt::gray,1));
        p.setBrush(QBrush(tb));

        p.drawEllipse(WS,HS,dia,dia);
    }
    else if(m_eLabelStyle == eLabelStyle::Rectangle && m_eLabelType == eLabelType::Color)
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing,true);
        QColor color = palette().color(QPalette::Normal,QPalette::Window);

        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(color));

        p.drawRect(this->rect());
    }
    else
    {
        QLabel::paintEvent(e);
    }
}

void DataLabel::fontSlot()
{
    setFont(*fatherFont);
}
