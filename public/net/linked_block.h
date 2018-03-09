/*
 * linked_block.h
 * 内存块
 */

#ifndef __LINKED_BLOCK_H__
#define __LINKED_BLOCK_H__

#include "platform.h"

class LinkedBlockPool;
class LinkedBlock
{
public:
    LinkedBlock(LinkedBlockPool* pool);
    ~LinkedBlock();

public:
    LinkedBlock* get_next();
    void set_next(LinkedBlock* next);

    uint32 available();
    uint32 remain();

    char* get_data();
    char* get_cursor();

    void fill(uint32 bytes);

    void reuse();
    void destroy();

private:
    LinkedBlock* next_;

    char* begin_;
    char* end_;
    char* cursor_;

    LinkedBlockPool* block_pool_;
};

#endif  // __LINKED_BLOCK_H__