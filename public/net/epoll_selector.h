/*
 * epoll_selector.h
 * epoll多路复用
 */

#ifndef __EPOLL_SELECTOR_H__
#define __EPOLL_SELECTOR_H__

#include "iselector.h"

#include <sys/epoll.h>


class EpollSelector : public ISelector
{
public:
    EpollSelector();
    ~EpollSelector();

public:
    virtual bool init();
    virtual bool destroy();
    virtual int select();
    virtual bool add(IHandler* handler, int flag);
    virtual bool remove(IHandler* handler);
    virtual bool modify(IHandler* handler, int flag);

private:
    int epollfd_;
};

#endif  // __EPOLL_SELECTOR_H__