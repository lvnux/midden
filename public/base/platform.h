/*
 * platfrom.h
 * 常用类型定义，兼容不同平台
 */

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdint.h>

#ifndef int8
typedef int8_t int8;
#endif

#ifndef uint8
typedef uint8_t uint8;
#endif

#ifndef int16
typedef int16_t int16;
#endif

#ifndef uint16
typedef uint16_t uint16;
#endif

#ifndef int32
typedef int32_t int32;
#endif

#ifndef uint32
typedef uint32_t uint32;
#endif

#ifndef int64
typedef int64_t int64;
#endif

#ifndef uint64
typedef uint64_t uint64;
#endif

#endif  // __PLATFORM_H__