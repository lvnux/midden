#ifndef __I_BUFFER_MANAGER_H__
#define __I_BUFFER_MANAGER_H__

#include "ibuffer_in.h"
#include "ibuffer_out.h"
#include "linked_block_pool.h"

class IBufferManager
{
public:
    virtual bool init() = 0;
    virtual bool destroy() = 0;
    virtual IBufferIn* create_buffer_in(LinkedBlockPool* pool) = 0;
    virtual IBufferOut* create_buffer_out(LinkedBlockPool* pool) = 0;
};

#endif   // __I_BUFFER_MANAGER_H__