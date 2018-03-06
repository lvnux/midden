#ifndef __I_BUFFER_OUT_H__
#define __I_BUFFER_OUT_H__

#include "output_buffer.h"
#include "iblock_out.h"
#include "iflush_handler.h"


class IBufferOut : public OutBuffer
{
public:
    virtual IBlockOut* get_block() = 0;
    virtual void set_flush_handler(IFlushHandler* handler) = 0;
};

#endif  // __I_BUFFER_OUT_H__