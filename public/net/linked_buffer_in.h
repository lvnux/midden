#ifndef __LINKED_BUFFER_IN_H__
#define __LINKED_BUFFER_IN_H__

#include "ibuffer_in.h"

class LinkedBufferIn : IBufferIn
{
public:
    virtual IBlockIn* get_block() = 0;

    virtual bool read(char* data, uint32 bytes);
    virtual uint32 available() const;
    virtual void mark();
    virtual void reset();
    virtual void close();
};

#endif  // __LINKED_BUFFER_IN_H__