/*
 * iblock_in.h
 * 接收缓存内存块，用于缓存接收到的消息
 */

#ifndef __I_BLOCK_IN_H__
#define __I_BLOCK_IN_H__

class IBlockIn
{
public:
    virtual char* data() = 0;
    virtual int size() = 0;
    virtual void fill(int bytes) = 0;
    virtual void release() = 0;
};

#endif  // __I_BLOCK_IN_H__