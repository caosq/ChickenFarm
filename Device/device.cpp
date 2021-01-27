#include "device.h"

Device::Device()
{

}

void Device::dataChanged(void* pvArg)
{
    emit valChanged(pvArg);
}
