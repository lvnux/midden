/*
 * linked_buffer_out.h
 * 待发送消息缓存
 */

#ifndef __LINKED_BUFFER_OUT_H__
#define __LINKED_BUFFER_OUT_H__

#include "ibuffer_out.h"
#include "linked_block.h"

class LinkedBlockPool;
class LinkedBufferOut : public IBufferOut
{
public:
    LinkedBufferOut(LinkedBlockPool* pool);
    ~LinkedBufferOut();

public:
    void consumer_block(uint32 length);

protected:
    virtual IBlockOut* get_block();
    virtual void set_flush_handler(IFlushHandler* handler);

    virtual void write(const char* data, uint32 bytes);
    virtual void flush();
    virtual uint32 size() const;
    virtual void close();

private:
    LinkedBlock* first_;
    LinkedBlock* last_;

    uint32 origin_;
    uint32 length_;  // 已写入数据总长度

    IFlushHandler* handler_;
    LinkedBlockPool* block_pool_;
};

#endif  // __LINKED_BUFFER_OUT_H__