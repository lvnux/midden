#include "linked_block.h"
#include "linked_block_pool.h"

#include <string.h>

#define BLOCK_MAX_SIZE 30000

LinkedBlock::LinkedBlock(LinkedBlockPool* pool)
{
    begin_ = new char[BLOCK_MAX_SIZE];
    memset(begin_, 0, BLOCK_MAX_SIZE);
    end_ = begin_ + BLOCK_MAX_SIZE;
    cursor_ = begin_;
    next_ = NULL;
    block_pool_ = pool;
}

LinkedBlock::~LinkedBlock()
{
    delete []begin_;
}

LinkedBlock* LinkedBlock::get_next()
{
    return next_;
}

void LinkedBlock::set_next(LinkedBlock* next)
{
    next_ = next;
}

uint32 LinkedBlock::available()
{
    return cursor_ - begin_;
}

uint32 LinkedBlock::remain()
{
    return end_ - cursor_;
}

char* LinkedBlock::get_data()
{
    return begin_;
}

char* LinkedBlock::get_cursor()
{
    return cursor_;
}

void LinkedBlock::fill(uint32 bytes)
{
    cursor_ += bytes;
    if (cursor_ > end_)
    {
        cursor_ = end_;
    }
}

void LinkedBlock::reuse()
{
    memset(begin_, 0, BLOCK_MAX_SIZE);
    cursor_ = begin_;
    next_ = NULL;
}

void LinkedBlock::destroy()
{
    reuse();
    block_pool_->put_block(this);
}