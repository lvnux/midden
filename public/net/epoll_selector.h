#ifndef __EPOLL_SELECTOR_H__
#define __EPOLL_SELECTOR_H__

#include "ihandler.h"

#include <sys/epoll.h>


class EpollSelector
{
public:
    enum SelectFlag
    {
        SELECT_READ = 0x01,
        SELECT_WRITE = 0x02,

        // 0x02 - 0x0f is reserved
        
        SELECT_ALL = 0x0f,
    };

public:
    EpollSelector();
    ~EpollSelector();

public:
    bool init();
    bool destroy();
    int select();
    bool add(IHandler* handler, int flag);
    bool remove(IHandler* handler);
    bool modify(IHandler* handler, int flag);

private:
    int epollfd_;
};

#endif  // __EPOLL_SELECTOR_H__