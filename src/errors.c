#include "wbtp/errors.h"
#include <stdlib.h>
#include <string.h>

char *wbtp_error = NULL;

char *wbtp_get_error(void)
{
    return wbtp_error;
}

void wbtp_set_error(const char *error)
{
    wbtp_error = (char *)error;
}
