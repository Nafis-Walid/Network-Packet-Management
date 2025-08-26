#ifndef __APP_H__
#define __APP_H__

#include "udp-packet.h"
#include "linked-list.h"
#include "hash-table.h"

#define PRESS_ANY_KEY_TO_CONTINUE  \
    do \
    {  \
        printf("Press any key to continue..."); \
        getchar(); \
    } while(0)

extern void run_packet_management_project();

#endif/*__APP_H__*/