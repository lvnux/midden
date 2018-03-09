/*
 * linked_buffer_in.h
 * 接收消息缓存
 */

#ifndef __LINKED_BUFFER_IN_H__
#define __LINKED_BUFFER_IN_H__

#include "ibuffer_in.h"
#include "linked_block.h"


class LinkedBlockPool;
class LinkedBufferIn : public IBufferIn
{
public:
    LinkedBufferIn(LinkedBlockPool* pool);
    virtual ~LinkedBufferIn();

public:
    void put_block(LinkedBlock* block, uint32 length);

protected:
    virtual IBlockIn* get_block();

    virtual bool read(char* data, uint32 bytes);
    virtual uint32 available() const;
    virtual void mark();
    virtual void reset();
    virtual void close();

private:
    LinkedBlock* first_;
    LinkedBlock* last_;
    LinkedBlock* current_;

    int origin_;
    int readed_;  // 已读取的数据长度
    int offset_;  // 
    int length_;  // 总的可读数据长度

    LinkedBlockPool* block_pool_;
};

#endif  // __LINKED_BUFFER_IN_H__