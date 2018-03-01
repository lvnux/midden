#ifndef __LINKED_BLOCK_OUT_H__
#define __LINKED_BLOCK_OUT_H__

#include "iblock_out.h"

class LinkedBlockOut : public IBlockOut
{
public:
    virtual const char* data() = 0;
    virtual int size() = 0;
    virtual void consume(int bytes) = 0;
    virtual void release() = 0;
};

#endif  // __LINKED_BLOCK_OUT_H__