#include "linked_buffer_in.h"
#include "linked_block_in.h"
#include "linked_block_pool.h"

#include <string.h>
#include <algorithm>


LinkedBufferIn::LinkedBufferIn(LinkedBlockPool* pool)
    : first_(NULL), last_(NULL), current_(NULL),
      origin_(0), readed_(0), offset_(0), length_(0),
      block_pool_(pool)
{
    
}

LinkedBufferIn::~LinkedBufferIn()
{
    LinkedBlock* block = first_;
    while (block != last_)
    {
        first_ = block->get_next();
        block->destroy();
        block = first_;
    }

    if (NULL != last_)
    {
        last_->destroy();
    }
}

void LinkedBufferIn::put_block(LinkedBlock* block, uint32 length)
{
    length_ += length;
    if (NULL == last_)
    {
        first_ = last_ = current_ = block;
    }
    else
    {
        last_->set_next(block);
        last_ = block;
    }
}

IBlockIn* LinkedBufferIn::get_block()
{
    if (NULL == last_ || last_->remain() < 2000)
    {
        return new LinkedBlockIn(block_pool_->get_block(), this);
    }
    else
    {
        return new LinkedBlockIn(last_, this, true);
    }
}

bool LinkedBufferIn::read(char* data, uint32 bytes)
{
    if (bytes > available())
    {
        return false;
    }

    readed_ += bytes;
    uint32 readed = 0;  // 已经读取多少数据
    int count = 0;  // 每次读取多少数据
    while (NULL != current_ && bytes > 0)
    {
        count = std::min(bytes, current_->available()-offset_);
        memcpy(data+readed, current_->get_data()+offset_, count);
        bytes -= count;
        offset_ += count;
        readed += count;
        if ((int)current_->available() == offset_ && current_ != last_)
        {
            current_ = current_->get_next();
            offset_ = 0;
        }
    }

    return true;
}

uint32 LinkedBufferIn::available() const
{
    return length_ - readed_;
}

void LinkedBufferIn::mark()
{
    LinkedBlock* block = first_;
    if (block != current_)
    {
        first_ = block->get_next();
        block->destroy();
        block = first_;
    }
    length_ -= readed_;
    readed_ = 0;
    origin_ = offset_;
}

void LinkedBufferIn::reset()
{
    offset_ = origin_;
    readed_ = 0;
    current_ = first_;
}

void LinkedBufferIn::close()
{
    delete this;
}