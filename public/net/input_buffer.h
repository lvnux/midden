/*
 * input_buffer.h
 * 接收数据缓存
 */

#ifndef __INPUT_BUFFER_H__
#define __INPUT_BUFFER_H__

#include "platform.h"

class InputBuffer
{
public:
    virtual bool read(char* data, uint32 bytes) = 0;
    virtual uint32 available() const = 0;
    virtual void mark() = 0;
    virtual void reset() = 0;
    virtual void close() = 0;
};

#endif  // __INPUT_BUFFER_H__