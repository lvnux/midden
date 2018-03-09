/*
 * linked_block_in.h
 * 接收消息内存存储管理
 */

#ifndef __LINKED_BLOCK_IN_H__
#define __LINKED_BLOCK_IN_H__

#include "iblock_in.h"
#include "linked_block.h"


class LinkedBufferIn;
class LinkedBlockIn : public IBlockIn
{
public:
    LinkedBlockIn(LinkedBlock* block, LinkedBufferIn* buffer, bool filled=false);
    virtual ~LinkedBlockIn();

protected:
    virtual char* data();
    virtual int size();
    virtual void fill(int bytes);
    virtual void release();

private:
    LinkedBlock* block_;
    LinkedBufferIn* buffer_;
    bool filled_;
};

#endif  // __LINKED_BLOCK_IN_H__