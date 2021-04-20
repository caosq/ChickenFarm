#include <QFontMetrics>
#include "modebutton.h"
#include "messagebox.h"

#define SP_X 3
#define SP_Y 3

#define M_BC "#27CEE0"
#define M_TC "#dfd8c8"
#define M_HL "#678db2"
#define M_HLTC "#ffffff"

privateMenu::menuStyle ModeButton::fatherStyle =
{
    QColor(M_BC),
    QColor(M_HL),
    QColor(M_TC),
    QColor(M_HLTC)
};

privateMenu::privateMenu(QWidget *parent) :
    QMenu(parent)
{
    setContentsMargins(-15,0,0,0);
    setMaximumWidth(parent->width());
    //setFixedWidth(parent->width());

    //setAutoFillBackground(false);
}

void privateMenu::setItem(int id, QString text)
{
    if( !_acmap.contains(id) )
    {
        QAction *temp = addAction(text,this,SLOT(actionSlot()));
        temp->setData(id);
        _acmap.insert(id,temp);
    }
    else
    {
        QAction *temp = _acmap.value(id);
        temp->setText(text);
    }
}

bool privateMenu::replaceItemId(int old_id, int new_id)
{
    if( !_acmap.contains(old_id) ){
        return false;
    }
    QAction *temp = _acmap.take(old_id);
    temp->setData(new_id);
    _acmap.insert(new_id,temp);

    return true;
}

void privateMenu::removeItem(int id)
{
    if( _acmap.contains(id) ){
        removeAction(_acmap.value(id));
        _acmap.remove(id);
    }
}

void privateMenu::setColorStyle(privateMenu::menuStyle style)
{
    setBackgroundColor(style._Background);
    setHighlightColor(style._Highlight);
    setHightlightTextColor(style._HightlightText);
    setTextColor(style._Text);  
}

void privateMenu::setBackgroundColor(QColor color)
{
    QPalette palette = this->palette();
    palette.setBrush(this->backgroundRole(),QBrush(color));
    palette.setBrush(QPalette::Normal,QPalette::Dark,QBrush(color));
    palette.setBrush(QPalette::Normal,QPalette::Light,QBrush(color));
    palette.setBrush(QPalette::Normal,QPalette::Button,QBrush(color));
    palette.setBrush(QPalette::Normal,QPalette::Shadow,QBrush(color));

    setPalette(palette);
}

void privateMenu::setHighlightColor(QColor color)
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Normal,QPalette::Highlight,QBrush(color));
    setPalette(palette);
}

void privateMenu::setTextColor(QColor color)
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Normal,QPalette::WindowText,QBrush(color));
    palette.setBrush(QPalette::Normal,QPalette::ButtonText,QBrush(color));
    setPalette(palette);
}

void privateMenu::setHightlightTextColor(QColor color)
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Normal,QPalette::HighlightedText,QBrush(color));
    setPalette(palette);
}

/*void privateMenu::setMenuStyleSheet()
{
    QString strStyle = "background-color: " + m_styleSheet._Background;

    setStyleSheet(strStyle);
}*/


void privateMenu::setCurrentItem(int id)
{
    if( _acmap.contains(id) ){
        setActiveAction(_acmap.value(id));
    }
}

QList<int> privateMenu::getValueList()
{
    return _acmap.keys();
}

QString privateMenu::currentText(int id)
{
    QString str = "";
    if( _acmap.contains(id) ){
        str = _acmap.value(id)->text();
    }
    return str;
}

void privateMenu::actionSlot()
{
    QAction *ac = qobject_cast<QAction *>(sender());
    emit valueChange(ac->data().toInt());
}

ModeButton* ModeButton::father = nullptr;
QFont ModeButton::fatherFont = textControl::instance()->font(textControl::size16);
QPixmap *ModeButton::pixmap = nullptr;

ModeButton::ModeButton(QWidget *parent) :
    Button(parent),m_pMonitor(nullptr)
{
    _dtype = Monitor::Uint16t;

    defValState = false;
    enableValMarker = true;
    save = true;
    automaticAttack = true;

    m_iDefaultVal = 0;
    m_iCurrentVal = 0;

    currentText = "";

    _alignmentFlag = Qt::AlignCenter;
    _margin = 0;
    _gap = 0;

    initMenu();

   // setPermission(fatherLevel);
    setFont(fatherFont);

    if(father)
    {
        connect(father,SIGNAL(fontChange()),this,SLOT(fontSlot()));
    }
    connect(_menu,SIGNAL(valueChange(int)),this,SLOT(valueSlot(int)));
    //connect(this,SIGNAL(pressed()),this,SLOT(clickedSlot()));

   // connect(ScreenSaver::instance(),SIGNAL(isTimeToBack()),this,SLOT(hideMenu()));
}

ModeButton* ModeButton::forefather()
{
    if(father == nullptr)
    {
        father = new ModeButton();
    }
    return father;
}

bool ModeButton::setMonitorData(void* pvVal, Monitor::DataType emDataType)
{
    m_pMonitor = DataMonitor::monitorRegist(pvVal, emDataType);

    if(m_pMonitor != nullptr)
    {
        connect(m_pMonitor, SIGNAL(valChanged(Monitor*)), this, SLOT(setValue(Monitor*)));
        setValue(m_pMonitor->getCurVal());

        return true;
    }
    return false;
}

void ModeButton::paintEvent(QPaintEvent *e)
{
    if( pixmap )
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
            painter.drawPixmap(1,1,this->width(),this->height(),*pixmap);
            painter.drawText(_margin-5,1,this->width()-_gap,this->height(),_alignmentFlag,getElidedText());
        }
        else
        {
            painter.drawPixmap(0,0,this->width(),this->height(),*pixmap);
            painter.drawText(_margin-5,0,this->width()-_gap,this->height(),_alignmentFlag,getElidedText());
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
        painter.drawText(_margin-5,0,this->width(),this->height(),_alignmentFlag,getElidedText());
    }

    if( !defValState && enableValMarker )
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::green);
        painter.drawEllipse(SP_X,SP_Y,5,5);
    }
}

void ModeButton::initMenu()
{
    _menu = new privateMenu(this);
    _menu->setFont(fatherFont);
   // _menu->setColorStyle(fatherStyle);
    _menu->setFixedWidth(this->width());

    _menu->setBackgroundColor(Qt::white);
    setMenu(_menu);
}

QString ModeButton::getElidedText()
{
    QFontMetrics metrics(this->font());
    currentText = _menu->currentText(m_iCurrentVal);

    return metrics.elidedText(currentText,Qt::ElideRight,this->width()-_gap);
}

void ModeButton::fontSlot()
{
    setFont(fatherFont);
}

bool ModeButton::clickedSlot()
{
    bool     xValue = 0;
    bool     xShowMsgBox = false;
    uint8_t  ucValue = 0;
    uint16_t usValue = 0;
    uint32_t uiValue  = 0;

    int8_t   cValue  = 0;
    int16_t  sValue  = 0;
    int32_t  iValue  = 0;

    int32_t  iCheckVal;
    QString  sChecktext;
    void*    pCheckValAddr = nullptr;
    Monitor::DataType eCheckDataType;

    if(m_xCheckMode)
    {
        for (uint8_t n = 0; n < m_BtnCheckDatas.count(); n++)
        {
            if(m_BtnCheckDatas[n]->pCheckValAddr == nullptr){continue;}

            eCheckDataType = m_BtnCheckDatas[n]->eCheckDataType;
            pCheckValAddr = m_BtnCheckDatas[n]->pCheckValAddr;
            sChecktext = m_BtnCheckDatas[n]->strChecktext;
            iCheckVal = m_BtnCheckDatas[n]->iCheckVal;

            if(eCheckDataType == Monitor::Uint8t)
            {
                ucValue = *static_cast<uint8_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(ucValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Uint16t)
            {
                usValue = *static_cast<uint16_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(usValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Int8t)
            {
                cValue = *static_cast<int8_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(cValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Int16t)
            {
                sValue = *static_cast<int16_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(sValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Uint32t)
            {
                uiValue = *static_cast<uint32_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(uiValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Int32t)
            {
                iValue = *static_cast<int32_t*>(pCheckValAddr);
                if(iCheckVal != int32_t(iValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
            else if(eCheckDataType == Monitor::Boolean)
            {
                xValue = *static_cast<bool*>(pCheckValAddr);
                if(iCheckVal != int32_t(xValue)){
                    xShowMsgBox = true;
                    break;
                }
            }
        }
    }
    if(xShowMsgBox && g_xDebugMode == false)
    {
        messageBox *pConfirmationBox =  new messageBox(messageBox::Information);
        pConfirmationBox->setButtonText(messageBox::Yes,"确认");
        pConfirmationBox->setInformativeText(sChecktext);
        pConfirmationBox->show();
        return false;
    }
    else
    {
        if(m_xConfirmMode)
        {
            int32_t test;
            messageBox *pConfirmationBox = new messageBox(messageBox::Question);
            pConfirmationBox->setButtonText(messageBox::Yes,"确认");
            pConfirmationBox->setButtonText(messageBox::No,"取消");
            pConfirmationBox->setInformativeText(m_sConfirmtext);
            test = pConfirmationBox->exec();

            if(test == messageBox::No){return false;}
        }
        if(m_xDelayMode)
        {
            m_DelayTimer.start(m_iDelayTimeMs);
        }
        emit buttonClicked();
    }
    return true;
}

void ModeButton::valueSlot(int tempVal)
{
    if(clickedSlot())
    {
        setValue(tempVal);
        update();
    }
}

void ModeButton::hideMenu()
{
    _menu->hide();
}

int32_t ModeButton::getCurrentValue()
{
    return m_iCurrentVal;
}

void ModeButton::setValue(Monitor* pMonitor)
{
    int32_t val = pMonitor->getCurVal();
    int intval = int32_t(val);
    m_iCurrentVal = intval;
    currentText = _menu->currentText(intval);
    defValState = (m_iDefaultVal == intval) ? true:false;
    update();
    emit valChanged(this);
}

void ModeButton::setValue(int32_t val)
{
    int intval = int32_t(val);
    m_iCurrentVal = intval;
    currentText = _menu->currentText(intval);
    defValState = (m_iDefaultVal == intval) ? true:false;

    if(m_pMonitor)
    {
        m_pMonitor->setValue(val);
    }
    else
    {
        emit valChanged(this);
    }
    update();
}

void ModeButton::setDefaultValue(int defValue)
{
    m_iDefaultVal = defValue;
    m_iCurrentVal = defValue;
    defValState = (m_iDefaultVal == m_iCurrentVal) ? true:false;
    setText(_menu->currentText(m_iCurrentVal));
    update();
}

void ModeButton::setTextColor(QPalette::ColorGroup group, QColor color)
{
    QPalette    palette = this->palette();
    palette.setBrush(group,QPalette::ButtonText,QBrush(color));
    setPalette(palette);
}

void ModeButton::setText(const QString &text)
{
    currentText = text;
    update();
}

void ModeButton::setItem(int id, QString text)
{
    _menu->setItem(id,text);
}

void ModeButton::removeItem(int id)
{
    _menu->removeItem(id);
}

void ModeButton::setFont(const QFont & font)
{
    QPushButton::setFont(font);
    _menu->setFont(font);
}

void ModeButton::setTextSize(int size)
{
    QFont font;
    font.setPointSize(size);
    setFont(font);
}

void ModeButton::setFatherFontSize(int size)
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

void ModeButton::setFatherPixmap(QString filePath)
{
    if(father == this)
    {
        if(pixmap)
        {
            pixmap->load(filePath);
        }
        else
        {
            pixmap = new QPixmap(filePath);
        }
    }
}

void ModeButton::setFatherItemStyle(privateMenu::menuStyle style)
{
    if(father == this)
    {
        fatherStyle = style;
    }
}

void ModeButton::setClicked()
{
    clickedSlot();
}

void ModeButton::setTextAlignment(Qt::Alignment flag)
{
    _alignmentFlag = flag;
}

void ModeButton::setTextMargin(int margin)
{
    _margin = margin;
}

void ModeButton::setButtumGap(int gap)
{
    _gap = gap;
}

