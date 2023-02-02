#ifndef UTIL_H_
#define UTIL_H_

#include "stdio.h"

#define UNIMPLEMENTED()                                             \
    do {                                                            \
        printf("UNIMPLEMENTED FUNCTION %s\n", __PRETTY_FUNCTION__); \
        while (true)                                                \
            ;                                                       \
    } while (0)

#define BIT(x) (1 << (x))
#define SET_BIT(x, y) ((x) |= BIT(y)))
#define CLEAR_BIT(x, y) ((x) &= ~BIT(y)))
#define CHECK_BIT(x, y) (((x)&BIT(y)) != 0)

#endif // UTIL_H_