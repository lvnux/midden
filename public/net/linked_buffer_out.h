#ifndef __LINKED_BUFFER_OUT_H__
#define __LINKED_BUFFER_OUT_H__

#include "ibuffer_out.h"

class LinkedBufferOut
{
public:
    LinkedBufferOut();
    ~LinkedBufferOut();
    public:
    virtual IBlockOut* get_block() = 0;
    virtual void set_flush_handler(IFlushHandler* handler) = 0;

    public:
    virtual void write(const char* data, uint32 bytes) = 0;
    virtual void flush() = 0;
    virtual uint32 size() const = 0;
    virtual void close() = 0;
};

#endif  // __LINKED_BUFFER_OUT_H__