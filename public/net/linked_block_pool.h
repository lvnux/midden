#ifndef __LINKED_BLOCK_POOL_H__
#define __LINKED_BLOCK_POOL_H__

#include <stack>

class LinkedBlock;
class LinkedBlockPool
{
public:
    LinkedBlockPool();
    ~LinkedBlockPool();

public:
    LinkedBlock* get_block();
    bool put_block(LinkedBlock* block);

private:
    typedef std::stack<LinkedBlock*> BlockStack;

private:
    BlockStack blocks_;
    uint32 max_count_;
};

#endif  // __LINKED_BLOCK_POOL_H__