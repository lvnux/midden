/*
 * output_buffer.h
 * 发送缓存基类
 */

#ifndef __OUTPUT_BUFFER_H__
#define __OUTPUT_BUFFER_H__

#include "platform.h"

class OutBuffer
{
public:
    virtual void write(const char* data, uint32 bytes) = 0;
    virtual void flush() = 0;
    virtual uint32 size() const = 0;
    virtual void close() = 0;
};

#endif  // __OUTPUT_BUFFER_H__