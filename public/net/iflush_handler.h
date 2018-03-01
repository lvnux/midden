#ifndef __I_FLUSH_HANDLER_H__
#define __I_FLUSH_HANDLER_H__


class IBufferOut;
class IFlushHandler
{
public:
    virtual void on_flush(IBufferOut* buffer) = 0;
};

#endif  // __I_FLUSH_HANDLER_H__