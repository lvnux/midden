#ifndef __LINKED_BUFFER_MANAGER_H__
#define __LINKED_BUFFER_MANAGER_H__

#include "ibuffer_manager.h"

class LinkedBufferManager : public IBufferManager
{
public:
    LinkedBufferManager();
    ~LinkedBufferManager();

public:
    virtual bool init();
    virtual bool destroy();
    virtual IBufferIn* create_buffer_in(LinkedBlockPool* pool);
    virtual IBufferOut* create_buffer_out(LinkedBlockPool* pool);
};

#endif  // __LINKED_BUFFER_MANAGER_H__