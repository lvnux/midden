#ifndef __LINKED_BLOCK_OUT_H__
#define __LINKED_BLOCK_OUT_H__

#include "iblock_out.h"


class LinkedBufferOut;
class LinkedBlockOut : public IBlockOut
{
public:
    LinkedBlockOut(const char* data, int size, LinkedBufferOut* buffer);
    virtual ~LinkedBlockOut();

public:
    virtual const char* data();
    virtual int size();
    virtual void consume(int bytes);
    virtual void release();

private:
    const char* data_;  // 数据起始地址
    int size_;  // 数据大小
    LinkedBufferOut* buffer_;
};

#endif  // __LINKED_BLOCK_OUT_H__