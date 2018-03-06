#include "linked_buffer_manager.h"
#include "linked_buffer_in.h"
#include "linked_buffer_out.h"


LinkedBufferManager::LinkedBufferManager()
{

}

LinkedBufferManager::~LinkedBufferManager()
{

}

bool LinkedBufferManager::init()
{
    return true;
}

bool LinkedBufferManager::destroy()
{
    return true;
}

IBufferIn* LinkedBufferManager::create_buffer_in(LinkedBlockPool* pool)
{
    return new LinkedBufferIn(pool);
}

IBufferOut* LinkedBufferManager::create_buffer_out(LinkedBlockPool* pool)
{
    return new LinkedBufferOut(pool);
}