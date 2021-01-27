#include "textcontrol.h"
//#include "languageTranslate/translate.h"

//#include "globalHeader/globalMark.h"

#ifdef PLATFORM_ARM
#define BASE_SIZE   10
#else
#define BASE_SIZE   6
#endif

#define SIZE_STEP   1
#define SIZE_LAVEL  30
#define PALETTE_COLOR "#dfd8c8"

//初始化静态变量
textControl *textControl::m_text = NULL;

//构造函数
textControl::textControl(QObject *parent) :
    QObject(parent)
{

    baseSize = BASE_SIZE;

    m_font.resize(SIZE_LAVEL);

    //创建一些列不同字号的QFont
    makeFont(baseSize);

    //设置调色板
    makePalette();

    //关联语言切换信号
    //connect(translate::instance(),SIGNAL(beforeReady()),this,SLOT(languageChangeSlot()));
}

textControl::~textControl()
{
    delete m_text;
}

void textControl::makeFont(int baseSize)
{
    int size = baseSize;
    for(int i = 0; i < m_font.size(); i++ )
    {
        m_font[i].setPointSize(size + (i * SIZE_STEP));
    }
}

//只要在控件中使用枚举textSize指定字号的控件才能生效
void textControl::languageChangeSlot()
{
    //中英切换时可以统一减小字号
    //makeFont();
}

void textControl::makePalette()
{
    m_brush = new QBrush(QColor(PALETTE_COLOR));
    m_brush->setStyle(Qt::SolidPattern);
    m_palette = new QPalette;

    //活动状态下windowText的颜色
    m_palette->setBrush(QPalette::Active,QPalette::WindowText,*m_brush);
    //非活动状态下windowText的颜色
    m_palette->setBrush(QPalette::Inactive,QPalette::WindowText,*m_brush);

     //活动状态下buttonText的颜色
    m_palette->setBrush(QPalette::Active,QPalette::ButtonText,*m_brush);
     //非活动状态下buttonText的颜色
    m_palette->setBrush(QPalette::Inactive,QPalette::ButtonText,*m_brush);

}

