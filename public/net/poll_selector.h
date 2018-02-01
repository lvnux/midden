/*
 * poll_selector.h
 * poll多路复用
 */

#ifndef __POLL_SELECTOR_H__
#define __POLL_SELECTOR_H__

#include "iselector.h"

#include <poll.h>
#include <limits.h>
#include <map>

#ifndef OPEN_MAX
#define OPEN_MAX 2048
#endif

class PollSelector : public ISelector
{
public:
    PollSelector();
    ~PollSelector();

public:
    virtual bool init();
    virtual bool destroy();
    virtual int select();
    virtual bool add(IHandler* handler, int flag);
    virtual bool remove(IHandler* handler);
    virtual bool modify(IHandler* handler, int flag);

private:
    std::map<int, IHandler*> fd_handler_;    // fd和handler的对应关系
    struct pollfd pollfds_[OPEN_MAX];
    int max_index_;  // pollfds_数组中有效fd的最大下标
};

#endif  // __POLL_SELECTOR_H__