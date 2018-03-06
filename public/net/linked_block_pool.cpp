#include "linked_block_pool.h"
#include "linked_block.h"


LinkedBlockPool::LinkedBlockPool()
    : max_count_(1024)
{

}

LinkedBlockPool::~LinkedBlockPool()
{
    while (!blocks_.empty())
    {
        LinkedBlock* block = blocks_.top();
        blocks_.pop();
        if (NULL != block)
        {
            delete block;
            block = NULL;
        }
    }
}

LinkedBlock* LinkedBlockPool::get_block()
{
    LinkedBlock* block = NULL;
    if (blocks_.size() > 0)
    {
        block = blocks_.top();
        blocks_.pop();
    }
    else
    {
        block = new LinkedBlock(this);
    }

    return block;
}

bool LinkedBlockPool::put_block(LinkedBlock* block)
{
    if (blocks_.size() < max_count_)
    {
        if (NULL == block)
        {
            return false;
        }
        block->reuse();
        blocks_.push(block);
    }
    else
    {
        if (NULL != block)
        {
            delete block;
            block = NULL;
        }
    }

    return true;
}