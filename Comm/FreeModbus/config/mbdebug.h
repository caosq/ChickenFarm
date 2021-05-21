#ifndef DEBUG_H
#define DEBUG_H

#include "mbconfig.h"
#include "stdio.h"

#define MB_DEBUG_ENABLE   1

#if MB_DEBUG_ENABLE  > 0

#if MB_LINUX_ENABLED  > 0

#endif

#if MB_UCOSIII_ENABLED  > 0
#include "my_rtt_printf.h"
#endif

int debug(const char *sFormat, ...);

#endif
#endif // DEBUG_H
