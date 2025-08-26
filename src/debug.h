#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#ifdef DEBUG
    #define DEBUG_BLOCK(code) do \
    { \
        printf("[DEBUG] (%s:%d - %s)\n", __FILE__, __LINE__, __func__); \
        code \
        printf("\n"); \
    } \
    while (0)

    #define DEBUG_PRINT(fmt, args...) \
        do { \
            fprintf(stderr, "[DEBUG] (%s:%d - %s)\n", __FILE__, __LINE__, __func__); \
            fprintf(stderr, fmt "\n\n", ##args); \
        } while (0)

    #define DEBUG_VAR(var) \
        do { \
            fprintf(stderr, "[DEBUG] (%s:%d - %s)\n", __FILE__, __LINE__, __func__); \
            fprintf(stderr, "%s = %d\n\n", #var, var); \
        } while (0)

#else
    #define DEBUG_BLOCK(code) do { } while (0)
    #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
    #define DEBUG_VAR(var) /* Don't do anything in release builds */
#endif


#endif /*__DEBUG_H__*/
