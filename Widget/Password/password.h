#ifndef PASSWORD_H
#define PASSWORD_H

#include <QObject>
#include <QDateTime>
#include <stdint-gcc.h>
#include <QTimer>
#include <QFile>
#include <QStringList>

#include "datasave.h"

class password : public QObject
{
    Q_OBJECT
public:
    enum GR_PWD{
        GR_USER,
        GR_FACTORY,
        GR_LEVEL1,
        GR_LEVEL2,
        GR_PROJECT,
        GR_SUPER,
        GR_ERROR
    };

    explicit password(QObject *parent = 0);

    static password *instance();

public:
    void init(){}

    void updateSnCore();
    void restartFpCounter();
    bool reloadFreqPassword(int new_pwd,int old_pwd);
    bool reloadUserPassword(int new_pwd,int old_pwd);

    bool judgementFreqPassword(uint32_t pwd);
    bool judgementPwd(GR_PWD pwdType,int pwd);
    bool judgementBarCorePassword(int passwd);

    //这5个函数不具备参数记忆功能，需要手动调用saveBarCode执行保存
    bool setBarCoreVersion(uint16_t val);
    bool setBarCoreUnitParameter(uint16_t val);
    bool setBarCoreContens(QList<uint16_t> valList);
    bool setBarCorePassword(QList<uint16_t> valList);
    bool setBarCoreLockState(uint16_t val);

    //void setBarCore(int addr,uint16_t val);
    void saveBarCode();

    uint16_t getBarCore(int addr);
    uint16_t getSnCore(int addr);
    QList<uint16_t> getBarCoreValue();
    QList<uint16_t> getSnCoreValue();

    QString getSnCoreText();
    QString getNewSnCoreText();
    QString getBarCoreText();
    uint32_t getFrequencyProtectPassword();

public slots:
    void updateCounter();

signals:
    void barCodeUpdata();
//    void unitBarCode(int,int);

private:
    void initValue();
    void initPassword();
    void updateBaseTime();
    void mathConversion();
    void createSn();
    void createPassword();
    void createSuperPassword();
    void createBarCode();

    bool checkPasswordFile();
    bool checkBarCoreFile();

    QString snCoreToStr(uint16_t val);
    uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);

    void savePassword();
    void readPassword();
    void saveOtherVar();

    void readBarCode();
    bool checkFile(DataSave *file = 0);

private:
    QTimer   *t_count;
    QDateTime   m_DateTime;
    static password *PwdHandler;

    int m_hour;
    int m_minute;
    int m_year;
    int m_month;
    int m_day;

    int Result_H;
    int Result_M;

    uint16_t    SN_H;
    uint16_t    SN_M;
    uint16_t    SN_L;

    uint16_t    SN0_H;
    uint16_t    SN0_M;
    uint16_t    SN0_L;

    static uint16_t fp_counter;

    //password
    static uint32_t UserPassword_Fixup;
    static uint32_t FrequencyProtectFix;
    static uint32_t FrequencyProtectDynamic;

    /*0 user;1 factory;2 level1;3 level2;4 project*/
    uint16_t m_Password[5];
    static uint32_t SuperPassword;

    uint16_t CounterFor20min;

    uint16_t CounterFor72Hour;//20min add 1 ,216 times is 72 hour
    uint16_t Reload72Hour;//72小时累计判断

    QString S_N_CORE;

    //QFile   *save_name;

    QVector<uint16_t> barCodeArray;
    /*
    1 user;2 factory;3 level1;4 level2;
    5 project;6 userfix;7 super;8 reload72; 9 counterfor72;10 frequencyProtectDynamic
    11 sn_code;
    */
    QString m_index[20];

    DataSave    *passwordFile;
    DataSave    *barCodeFile;

};

#endif // PASSWORD_H
