#include "device.h"

Device::Device(QWidget *parent) :
    QWidget(parent)
{

}

void Device::dataChanged(void* pvArg)
{
    emit valChanged(pvArg);
}
