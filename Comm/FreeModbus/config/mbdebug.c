#include "mbdebug.h"

int debug(const char *sFormat, ...)
{
#if MB_LINUX_ENABLED
    printf(sFormat);

#elif MB_UCOSIII_ENABLED
    myprintf(sFormat);

#endif
}
