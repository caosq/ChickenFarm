#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class Device : public QWidget
{
    Q_OBJECT

public:
    typedef struct
    {
        QFrame*  IsRunning = nullptr;
        QFrame*  IsError = nullptr;
    }sQFrameState;

public:
    explicit Device(QWidget *parent = nullptr);
    void dataChanged(void* pvArg);

signals:
    void valChanged(void* pvArg);

public:
     sQFrameState m_sQFrameState;
     uint16_t   m_usDeviceIndex = 0;          //设备标号
};

#endif // DEVICE_H
