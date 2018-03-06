#include "linked_buffer_out.h"
#include "linked_block_pool.h"
#include "linked_block_out.h"
#include "net_socket.h"

#include <string.h>
#include <algorithm>


LinkedBufferOut::LinkedBufferOut(LinkedBlockPool* pool)
    : first_(NULL), last_(NULL), origin_(0), length_(0),
      block_pool_(pool)
{

}

LinkedBufferOut::~LinkedBufferOut()
{

}

void LinkedBufferOut::consumer_block(uint32 length)
{
    if (length > length_)
    {
        return;
    }

    if (first_->available() > length + origin_)
    {
        origin_ += length;
    }
    else
    {
        LinkedBlock* block = first_->get_next();
        first_->destroy();  // 释放空间，还回池子
        first_ = block;
        if (NULL == first_)
        {
            last_ = NULL;
        }
        origin_ = 0;
    }
}


IBlockOut* LinkedBufferOut::get_block()
{
    if (NULL == first_)
    {
        return new LinkedBlockOut(NULL, 0, this);
    }
    else
    {
        return new LinkedBlockOut(first_->get_data()+origin_, first_->available()-origin_, this);
    }
}

void LinkedBufferOut::set_flush_handler(IFlushHandler* handler)
{
    handler_ = handler;
}

void LinkedBufferOut::write(const char* data, uint32 bytes)
{
    if (0 == bytes)
    {
        return;
    }

    length_ += bytes;
    int count = 0;  // 一次写入多少数据
    while (bytes != 0)
    {
        if (NULL == last_ || last_->remain() == 0)
        {
            first_ = last_ = block_pool_->get_block();
        }
        {
            last_->set_next(block_pool_->get_block());
            last_ = last_->get_next();
        }

        count = std::min(bytes, last_->remain());
        memcpy(last_->get_cursor(), data, count);
        bytes -= count;
        data += count;
    }
}

void LinkedBufferOut::flush()
{
    if (NULL != handler_)
    {
        handler_->on_flush(this);
    }
}
uint32 LinkedBufferOut::size() const
{
    return length_;
}

void LinkedBufferOut::close()
{
    delete this;
}