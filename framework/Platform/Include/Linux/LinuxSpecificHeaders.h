#ifndef LINUXSPECIFICHEADERS_H
#define LINUXSPECIFICHEADERS_H

#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/ioctl.h> 
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SET_THIS_FILE_NAME() \
    static const char* const __THIS_FILE_NAME__ = \
    strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__;

#endif
