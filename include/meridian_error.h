#ifndef MERIDIAN_ERROR_H
#define MERIDIAN_ERROR_H

#include <stdio.h>

#define MERIDIAN_ERROR(...) do {\
    printf("ERROR: ");\
    printf(__VA_ARGS__);\
    printf("\n");\
} while(0)

#define MERIDIAN_WARNING(...) do {\
    printf("WARNING: ");\
    printf(__VA_ARGS__);\
    printf("\n");\
} while(0)

#define MERIDIAN_INFO(...) do {\
    printf("INFO: ");\
    printf(__VA_ARGS__);\
    printf("\n");\
} while(0);

#endif//MERIDIAN_ERROR_H
