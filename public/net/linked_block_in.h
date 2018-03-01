#ifndef __LINKED_BLOCK_IN_H__
#define __LINKED_BLOCK_IN_H__

#include "iblock_in.h"

class LinkedBlockIn : IBlockIn
{
public:
    virtual char* data() = 0;
    virtual int size() = 0;
    virtual void fill(int bytes) = 0;
    virtual void release() = 0;
};

#endif  // __LINKED_BLOCK_IN_H__