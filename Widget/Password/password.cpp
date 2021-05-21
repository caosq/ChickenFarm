#include "password.h"

#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include <QDebug>

#define MAX_USER_VAL    100000000
#define FILE_LOCATION   "/home/root/.pp.ini"
#define BAR_CODE    "/home/root/.b_code.ini"

uint16_t    password::fp_counter = 0;
uint32_t    password::UserPassword_Fixup = 101010;
uint32_t    password::SuperPassword = 361903;
uint32_t    password::FrequencyProtectFix = 921387;
uint32_t    password::FrequencyProtectDynamic = 971296;

password *password::PwdHandler = NULL;

password::password(QObject *parent) :
    QObject(parent)
{
    initValue();

    initPassword();

    connect(t_count,SIGNAL(timeout()),SLOT(updateCounter()));
}

password *password::instance()
{

    if(PwdHandler == NULL){
        PwdHandler = new password;
    }

    return PwdHandler;

}

bool password::reloadFreqPassword(int new_pwd, int old_pwd)
{
    if( old_pwd == FrequencyProtectDynamic || old_pwd == FrequencyProtectFix ){
        if( new_pwd >= MAX_USER_VAL || new_pwd < 0 )
            return false;
        FrequencyProtectDynamic = new_pwd;
        saveOtherVar();
        return true;
    }
    else
        return false;
}

bool password::reloadUserPassword(int new_pwd,int old_pwd)
{
    if ( judgementPwd(GR_USER,old_pwd) || judgementPwd(GR_FACTORY,old_pwd)){
        if( new_pwd >= MAX_USER_VAL || new_pwd < 0)
            return false;
        UserPassword_Fixup = new_pwd;
        saveOtherVar();
        return true;
    }
    else
        return false;
}

bool password::judgementFreqPassword(uint32_t pwd)
{
    if ( pwd == FrequencyProtectFix || FrequencyProtectDynamic == pwd )
    {
        return true;
    }
    else
        return false;
}

bool password::judgementPwd(GR_PWD pwdType, int pwd)
{
    bool temp = false;
    if( pwd < 0 )
    {
        return temp;
    }
    uint8_t Judeg_temp = Reload72Hour;

    switch(pwdType)
    {
    case GR_USER:
    {
        if( pwd == UserPassword_Fixup || pwd == SuperPassword )
        {
            temp = true;
        }
        else if(pwd == m_Password[0] )
        {
            Reload72Hour |= 0x01;
            temp = true;
        }
    }
        break;
    case GR_FACTORY:
    {
        if( pwd == m_Password[1] || pwd == SuperPassword )
        {
            Reload72Hour |= 0x02;
            temp = true;
        }
    }
        break;
    case GR_LEVEL1:
    {
        if( pwd == m_Password[2] )
        {
            Reload72Hour |= 0x04;
            temp = true;
        }
    }
        break;
    case GR_LEVEL2:
    {
        if( pwd == m_Password[3] )
        {
            Reload72Hour |= 0x08;
            temp = true;
        }
    }
        break;
    case GR_PROJECT:
    {
        if( pwd == m_Password[4] )
            temp = true;
    }
        break;
    case GR_SUPER:
    {
        if( pwd == SuperPassword )
            temp = true;
    }
        break;
    default:
        break;

    }

    if( Judeg_temp != Reload72Hour ){
        CounterFor72Hour = 0;
        CounterFor20min = 0;
        saveOtherVar();
    }

    return temp;
}


//void password::setBarCore(int addr, uint16_t val)
//{
//    if( addr >= barCodeArray.size() )
//        return;

//    if( addr == 9 )
//    {
//        val = val & 0xff00;
//    }

//    barCodeArray[addr] = val;

//    emit unitBarCode(addr,val);
//}

uint16_t password::getBarCore(int addr)
{
    if( addr >= barCodeArray.size() ){
        return 0;
    }

    return barCodeArray.at(addr);
}

uint16_t password::getSnCore(int addr)
{
    if( addr == 0 ){
        return SN_H;
    }else if( addr == 1 ){
        return SN_M;
    }else if( addr == 2 ){
        return SN_L;
    }else{
        return 0;
    }
}

QList<uint16_t> password::getBarCoreValue()
{
    return barCodeArray.toList();
}

QList<uint16_t> password::getSnCoreValue()
{
    QList<uint16_t> list;
    list<<SN_H<<SN_M<<SN_L;
    return list;
}

bool password::judgementBarCorePassword(int passwd)
{
    int barCodePwd = 0;
    uint8_t height = 0;
    uint8_t low = 0;
    uint8_t zero = '0';

    for( int i = 10 ; i < 13; i++ ){
        height = (uint8_t)(barCodeArray.at(i) >> 8) - zero;
        barCodePwd = barCodePwd * 10 + height;
        low = (uint8_t)(barCodeArray.at(i) & 0x00ff) - zero;
        barCodePwd = barCodePwd * 10 + low;
    }
    if( passwd == barCodePwd )
        return true;
    else
        return false;
}

bool password::setBarCoreVersion(uint16_t val)
{
    if( val >= 10 && val <= 255 ){
        barCodeArray[0] = val;
        return true;
    }else{
        return false;
    }
}

bool password::setBarCoreUnitParameter(uint16_t val)
{
    if( val == 0xaa || val == 0x55 ){
        barCodeArray[1] = val;
        return true;
    }else{
        return false;
    }
}

bool password::setBarCoreContens(QList<uint16_t> valList)
{
    if( valList.size() != 8 ){
        return false;
    }else{
        for(int i = 0; i < valList.size(); i++ ){
            barCodeArray[i + 2] = valList.at(i);
        }
        emit barCodeUpdata();
        return true;
    }
}

bool password::setBarCorePassword(QList<uint16_t> valList)
{
    if( valList.size() != 3 ){
        return false;
    }else{
        for(int i = 0; i < valList.size(); i++){
            barCodeArray[i + 10] = valList.at(i);
        }
        return true;
    }
}

bool password::setBarCoreLockState(uint16_t val)
{
    if( val == 0xaa || val == 0x55 ){
        barCodeArray[13] = val;
        return true;
    }else{
        return false;
    }
}

void password::initValue()
{
    Reload72Hour = 0x00;
    CounterFor72Hour = 0;
    CounterFor20min = 0;

    barCodeArray.resize(15);

    barCodeArray[0] = 0x0a;
    barCodeArray[1] = 0x55;
    barCodeArray[10] = 0x5f5f;
    barCodeArray[11] = 0x5f5f;
    barCodeArray[12] = 0x5f5f;
    barCodeArray[13] = 0x55;

    for( int i = 0; i < 20; i++ ){
        m_index[i] = QString::number(i);
    }

    unsigned int temp;
    time_t the_time;
    the_time = time((time_t *)0);

    temp = the_time % 65535;
    fp_counter = (uint16_t)temp;

    t_count = new QTimer;
   // t_count->start(1000);

    passwordFile = new DataSave(FILE_LOCATION,QSettings::IniFormat);
    barCodeFile = new DataSave(BAR_CODE,QSettings::IniFormat);

}

void password::initPassword()
{
    updateBaseTime();

    mathConversion();

    createSn();

    createPassword();

    createSuperPassword();

    readPassword();

    readBarCode();
}

bool password::checkFile(DataSave *file)
{
    bool ret = false;
    if( file ){
        if( file->allKeys().size() == 0 ){
            ret = false;
        }else{
            ret = true;
        }
    }
    return ret;
}

uint32_t password::getFrequencyProtectPassword()
{
    return FrequencyProtectDynamic;
}

void password::readPassword()
{
    int i = 0;

    bool ret = false;
    ret = checkPasswordFile();

    for( i = 1; i < 6; i++ ){
        m_Password[i - 1] = passwordFile->value(m_index[i]).toInt();
    }

    UserPassword_Fixup = passwordFile->value(m_index[i++],UserPassword_Fixup).toInt();
    //SuperPassword = passwordFile->value(m_index[i++]).toInt();
    Reload72Hour = passwordFile->value(m_index[i++]).toInt();
    CounterFor72Hour = passwordFile->value(m_index[i++]).toInt();
    FrequencyProtectDynamic = passwordFile->value(m_index[i++]).toInt();
    S_N_CORE = passwordFile->value(m_index[i++],S_N_CORE).toString();
}

bool password::checkPasswordFile()
{
    if( !checkFile(passwordFile) ){
        qDebug()<<"password file empty start save file";
        savePassword();
        saveOtherVar();
        return false;
    }else{
        return true;
    }
}

bool password::checkBarCoreFile()
{
    if( !checkFile(barCodeFile) ){
        qDebug()<<"bar code file empty start save file";
        createBarCode();
        saveBarCode();
        return false;
    }else{
        return true;
    }
}

void password::savePassword()
{
    int i = 0;

    for( i = 1; i < 6; i++ ){
        passwordFile->setValueTardy(m_index[i],m_Password[i - 1]);
    }

    passwordFile->setValueTardy(m_index[i++],UserPassword_Fixup);
    passwordFile->setValueTardy(m_index[i++],SuperPassword);
    passwordFile->setValueTardy(m_index[i++],Reload72Hour);
    passwordFile->setValueTardy(m_index[i++],CounterFor72Hour);
    passwordFile->setValueTardy(m_index[i++],FrequencyProtectDynamic);
    passwordFile->setValueTardy(m_index[i++],S_N_CORE);

    passwordFile->syncFile();
}

void password::saveOtherVar()
{
    passwordFile->setValueTardy(m_index[6],UserPassword_Fixup);
    passwordFile->setValueTardy(m_index[8],Reload72Hour);
    passwordFile->setValueTardy(m_index[9],CounterFor72Hour);
    passwordFile->setValueTardy(m_index[10],FrequencyProtectDynamic);

    passwordFile->syncFile();
}

void password::saveBarCode()
{
    for( int i  = 0; i < 14; i++ ){
        barCodeFile->setValueTardy(QString::number(i),barCodeArray.at(i));
    }
    barCodeFile->syncFile();
}

void password::readBarCode()
{
    bool ret = false;
    ret = checkBarCoreFile();

    for( int i  = 0; i < 14; i++ ){
        barCodeArray[i] = barCodeFile->value(QString::number(i)).toInt();
    }
}

void password::restartFpCounter()
{
    if( t_count->isActive() )
        t_count->stop();
    t_count->start(1000);
}

void password::updateBaseTime()
{
    m_DateTime = QDateTime::currentDateTime();

    m_year = m_DateTime.date().year();
    m_month = m_DateTime.date().month();
    m_day = m_DateTime.date().day();

    m_hour = m_DateTime.time().hour();
    m_minute = m_DateTime.time().minute();
}

void password::mathConversion()
{
    Result_H = m_year - m_month*100 - m_day;
    Result_M = m_year + m_month*100 + m_day;

    SN_H = (uint16_t)Result_H;
    SN_M = (uint16_t)Result_M;
    SN_L = fp_counter;

    SN0_H = SN_H;
    SN0_M = SN_M;
    SN0_L = SN_L;

    uint16_t temp=0;

    temp = SN_H >> 8;
    SN_H = SN_H << 8;
    SN_H = SN_H | temp;

    temp = SN_M >> 8;
    SN_M = SN_M << 8;
    SN_M = SN_M | temp;

    temp = SN_L >> 8;
    SN_L = SN_L << 8;
    SN_L = SN_L | temp;

}

void password::createSn()
{
    S_N_CORE = "";
    S_N_CORE += snCoreToStr(SN_H);
    S_N_CORE += "-";
    S_N_CORE += snCoreToStr(SN_M);
    S_N_CORE += "-";
    S_N_CORE += snCoreToStr(SN_L);
}

QString password::snCoreToStr(uint16_t val)
{
    QString temp = "";
    temp = temp.sprintf("%.4x",val);
    temp = temp.toUpper();
    return temp;
}

void password::createSuperPassword()
{
/*    uint16_t temp16[3] = {SN_H,SN_M,SN_L};
    uint8_t temp8[6] = {0};
    uint8_t yy8[6] = {0};

    uint32_t sum_m = 0;

    int i = 0;
    for( i = 0; i < 6; i+=2 ){
        temp8[i] = temp16[i/2] >> 8;
        temp8[i+1] = temp16[i/2] & 0xff;
    }

    for( i = 0; i < 6; i++ ){
        yy8[i] += ( temp8[i] >> 4 );
        yy8[i] += ( temp8[i] & 0x0f );
        yy8[i] += 1;
        yy8[i] %= 10;
    }

    for( i = 0; i < 6; i++ ){
        sum_m *= 10;
        sum_m += yy8[i];
    }
*/
    //SuperPassword = (sum_m * 11)%(1000 * 1000);
}

void password::createBarCode()
{
    barCodeArray[2] = 0x4141;
    barCodeArray[3] = 0x3535;

    qsrand(QTime::currentTime().msec());

    int i = 4;
    uint16_t tempNum1 = 0;
    uint16_t tempNum2 = 0;
    for(i = 4; i < 8; i++){
        tempNum1 = qrand()%10;
        tempNum2 = qrand()%10;
        barCodeArray[i] = (tempNum1 + 48) << 8;
        barCodeArray[i] |= (tempNum2 + 48);
    }
    tempNum1 = qrand()%10;
    barCodeArray[8] = (tempNum1 + 48) << 8;
    barCodeArray[8] |= 0x005f;
    barCodeArray[9] = 0x5f00;

    barCodeArray[10]  = 0x5f5f;
    barCodeArray[11]  = 0x5f5f;
    barCodeArray[12]  = 0x5f5f;

}

void password::createPassword()
{
    uint16_t temp16[3] = {SN0_H,SN0_M,SN0_L};
    uint8_t temp_8[6];

    int i = 0;
    int j = 0;

    for( i = 0; i < 5; i++ ){
        temp16[0] += 1;
        temp16[1] += 1;
        temp16[2] += 1;

        for(j = 0; j < 6; j+=2){
            temp_8[j] = (temp16[j/2] & 0xff);
            temp_8[j+1] = (temp16[j/2] >> 8);
        }
        m_Password[i] = crc16(temp_8,6);
    }
}

void password::updateSnCore()
{
    updateBaseTime();

    mathConversion();

    createSn();

    createPassword();

    createSuperPassword();

    CounterFor72Hour = 0;
    CounterFor20min = 0;
    Reload72Hour = 0x00;

    savePassword();
}

QString password::getSnCoreText()
{
    return S_N_CORE;
}

QString password::getNewSnCoreText()
{
    updateSnCore();
    return S_N_CORE;
}

QString password::getBarCoreText()
{
    QString temp = "";
    QString code = "";
    QString barCode = "";
    char data[2] = {0};

    for( int i = 2; i < 10; i++){
        data[0] = (uint8_t)(barCodeArray.at(i) >> 8);
        data[1] = (uint8_t)(barCodeArray.at(i) & 0x00ff);

        temp = QString::fromUtf8(data,2);
        code = QString::fromLocal8Bit(temp.toLocal8Bit());

        barCode.append(code);
        barCode.append(" ");
    }
    data[0] = 0;
    data[1] = 0;

    temp = QString::fromUtf8(data,1);
    code = QString::fromLocal8Bit(temp.toLocal8Bit());

    return barCode.remove(code);
}

void password::updateCounter()
{
    CounterFor20min++;
    fp_counter++;

    if( 1200 == CounterFor20min ){
        CounterFor72Hour++;
        CounterFor20min = 0;
        saveOtherVar();
        if( 216 == CounterFor72Hour ){
            updateSnCore();
        }
    }
}


/* Table of CRC values for high-order byte */
static uint8_t table_crc_hi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static uint8_t table_crc_lo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

uint16_t password::crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i; /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}
