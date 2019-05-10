#include "linked_block_in.h"
#include "linked_buffer_in.h"


LinkedBlockIn::LinkedBlockIn(LinkedBlock* block, LinkedBufferIn* buffer, bool filled)
    : block_(block), buffer_(buffer), filled_(filled)
{

}

LinkedBlockIn::~LinkedBlockIn()
{
    if (!filled_)
    {
        block_->destroy();
    }
}

char* LinkedBlockIn::data()
{
    return block_->get_cursor();
}

int LinkedBlockIn::size()
{
    return block_->remain();
}

void LinkedBlockIn::fill(int bytes)
{
    if (bytes <= size())
    {
        block_->fill(bytes);
        buffer_->put_block(block_, bytes);
        filled_ = true;
    }
}

void LinkedBlockIn::release()
{
    delete this;
}