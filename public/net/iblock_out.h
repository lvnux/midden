/*
 * iblcok_out.h
 * 发送缓存内存块，用于缓存待发送数据
 */

#ifndef __I_BLOCK_OUT_H__
#define __I_BLOCK_OUT_H__

class IBlockOut
{
public:
    virtual const char* data() = 0;
    virtual int size() = 0;
    virtual void consume(int bytes) = 0;
    virtual void release() = 0;
};

#endif  // __I_BLOCK_OUT_H__