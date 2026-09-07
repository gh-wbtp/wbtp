#pragma once

#include "wbtp/winshit.h"

extern char *wbtp_error;

WBTP_API char *wbtp_get_error(void);

WBTP_API void wbtp_set_error(const char *error);
