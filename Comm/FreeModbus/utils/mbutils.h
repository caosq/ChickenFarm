#ifndef _MB_UTILS_H
#define _MB_UTILS_H

#include <stdint.h>

void  vMBTimeDly(uint16_t seconds, uint16_t milli);

uint16_t MB_CRCCalc(const uint8_t *Data,uint8_t StartIndex, uint16_t DataSize);
uint16_t usMBCRC16(uint8_t * pucFrame, uint16_t usLen);

#endif