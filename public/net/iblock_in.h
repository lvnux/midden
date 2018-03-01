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