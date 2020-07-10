#ifndef RTEMS_H
#define RTEMS_H

#ifndef BOOLEAN
    typedef unsigned char BOOLEAN;
#endif

#ifndef TRUE
    #define TRUE            1
#endif

#ifndef FALSE
    #define FALSE           0
#endif

#ifndef INT8U
    typedef unsigned char INT8U;
#endif

#ifndef INT16U
    typedef unsigned short INT16U;
#endif

#ifndef INT32U
    typedef unsigned int INT32U;
#endif

#ifndef INT64U
    typedef unsigned long long INT64U;
#endif

#ifndef INT8S
    typedef signed char INT8S;
#endif

#ifndef INT16S
    typedef signed short INT16S;
#endif

#ifndef INT32S
    typedef signed int INT32S;
#endif

#ifndef INT64S
    typedef signed long long INT64S;
#endif

#ifndef FP32
    typedef float FP32;
#endif

#ifndef FP64
    typedef double FP64;
#endif

#ifndef CHAR
    typedef char CHAR;
#endif

#ifndef char_t
    typedef char char_t;
#endif

#ifndef uint8_t
    typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
    typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
    typedef unsigned int uint32_t;
#endif

#ifndef uint64_t
    typedef unsigned long long uint64_t;
#endif

#ifndef int8_t
    typedef signed char int8_t;
#endif

#ifndef int16_t
    typedef signed short int16_t;
#endif

#ifndef int32_t
    typedef signed int int32_t;
#endif

#ifndef int64_t
    typedef signed long long int64_t;
#endif

#endif
