#include "linked_block_out.h"
#include "linked_buffer_out.h"


LinkedBlockOut::LinkedBlockOut(const char* data, int size, LinkedBufferOut* buffer)
    : data_(data), size_(size), buffer_(buffer)
{

}

LinkedBlockOut::~LinkedBlockOut()
{

}

const char* LinkedBlockOut::data()
{
    return data_;
}

int LinkedBlockOut::size()
{
    return size_;
}

void LinkedBlockOut::consume(int bytes)
{
    if (bytes <= size())
    {
        buffer_->consumer_block(bytes);
    }
}

void LinkedBlockOut::release()
{
    delete this;
}