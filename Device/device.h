#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device();

    void dataChanged(void* pvArg);
signals:
    void valChanged(void* pvArg);

};

#endif // DEVICE_H
