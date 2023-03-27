/*****************************************************************************/
/* STDINT.H                                                                  */
/* 仅适用于51单片机                                                           */
/*****************************************************************************/

#ifndef STDINT_H
#define STDINT_H

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;

typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;

typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int16_t int_fast16_t;
typedef uint16_t uint_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast32_t;

typedef int32_t intmax_t;
typedef uint32_t uintmax_t;

#define INT8_MAX 0x7f
#define INT8_MIN (-INT8_MAX - 1)
#define UINT8_MAX 0xff

#define INT16_MAX 0x7fff
#define INT16_MIN (-INT16_MAX - 1)
#define UINT16_MAX 0xffff

#define INT32_MAX 0x7fffffff
#define INT32_MIN (-INT32_MAX - 1)
#define UINT32_MAX 0xffffffff

#define INT_LEAST8_MAX (INT8_MAX)
#define INT_LEAST8_MIN (INT8_MIN)
#define UINT_LEAST8_MAX (UINT8_MAX)
#define INT_LEAST16_MAX (INT16_MAX)
#define INT_LEAST16_MIN (INT16_MIN)
#define UINT_LEAST16_MAX (UINT16_MAX)
#define INT_LEAST32_MAX (INT32_MAX)
#define INT_LEAST32_MIN (INT32_MIN)
#define UINT_LEAST32_MAX (UINT32_MAX)

#define INT_FAST8_MAX (INT8_MAX)
#define INT_FAST8_MIN (INT8_MIN)
#define UINT_FAST8_MAX (UINT8_MAX)
#define INT_FAST16_MAX (INT16_MAX)
#define INT_FAST16_MIN (INT16_MIN)
#define UINT_FAST16_MAX (UINT16_MAX)
#define INT_FAST32_MAX (INT32_MAX)
#define INT_FAST32_MIN (INT32_MIN)
#define UINT_FAST32_MAX (UINT32_MAX)

#define INT8_C(value) ((int_least8_t)(value))
#define UINT8_C(value) ((uint_least8_t)(value))
#define INT16_C(value) ((int_least16_t)(value))
#define UINT16_C(value) ((uint_least16_t)(value))
#define INT32_C(value) ((int_least32_t)(value))
#define UINT32_C(value) ((uint_least32_t)(value))
#define INTMAX_C(value) ((intmax_t)(value))
#define UINTMAX_C(value) ((uintmax_t)(value))

#endif
