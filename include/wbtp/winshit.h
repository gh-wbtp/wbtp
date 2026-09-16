#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(WBTP_STATIC)
#define WBTP_API
#elif defined(WBTP_BUILD)
#define WBTP_API __declspec(dllexport)
#else
#define WBTP_API __declspec(dllimport)
#endif
#else
#define WBTP_API
#endif
