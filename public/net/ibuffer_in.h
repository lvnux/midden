#ifndef __I_BUFFER_IN_H__
#define __I_BUFFER_IN_H__

#include "input_buffer.h"
#include "iblock_in.h"

class IBufferIn : public InputBuffer
{
public:
    virtual IBlockIn* get_block() = 0;
};

#endif  // __I_BUFFER_IN_H__