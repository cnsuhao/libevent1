#ifndef WINDOWSSPECIFICHEADERS_H
#define WINDOWSSPECIFICHEADERS_H

#include <winsock2.h>
#include <Windows.h>
#include <Inaddr.h>
#include <Iphlpapi.h>

#include "stdint.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define SET_THIS_FILE_NAME() \
    static const char* const __THIS_FILE_NAME__ = \
    strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__;


#endif