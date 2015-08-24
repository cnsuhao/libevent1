#ifndef SYSBIOSSPECIFICHEADERS_H
#define SYSBIOSSPECIFICHEADERS_H

//#include <xdc/std.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Mailbox.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define SET_THIS_FILE_NAME() \
    static const char* const __THIS_FILE_NAME__ = \
    strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__;

UInt  ConvertMicroSecondsToSystemTick(int64_t microSeconds);


#endif
