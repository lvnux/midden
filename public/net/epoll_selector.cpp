#include "epoll_selector.h"
#include "platform.h"

#include <unistd.h>
#include <string.h>

const int MAX_EVENTS = 3000;

EpollSelector::EpollSelector()
    : epollfd_(-1)
{

}

EpollSelector::~EpollSelector()
{

}

bool EpollSelector::init()
{
    if (-1 == epollfd_)
    {
        epollfd_ = epoll_create(65535);
    }

    return epollfd_ != -1;
}

bool EpollSelector::destroy()
{
    if (epollfd_ != -1)
    {
        close(epollfd_);
    }

    return true;
}

int EpollSelector::select()
{
    struct epoll_event events[MAX_EVENTS];
    memset(&events, 0, sizeof(epoll_event) * MAX_EVENTS);

    int count = epoll_wait(epollfd_, events, MAX_EVENTS, 0);
    uint32 event;
    for (int i = 0; i < count; i++)
    {
        IHandler* handler = reinterpret_cast<IHandler*>(events[i].data.ptr);
        event = events[i].events;
        if ((event & EPOLLIN) ||
            (event & EPOLLERR) ||
            (event & EPOLLHUP))
        {
            handler->on_read();
        }

        if (event & EPOLLOUT)
        {
            handler->on_write();
        }
    }

    return count;
}

bool EpollSelector::add(IHandler* handler, int flag)
{
    struct epoll_event event;
    event.data.ptr = handler;
    event.events = EPOLLET;  // 设置为边缘触发模式
    if (flag & SELECT_READ)
    {
        event.events |= EPOLLIN;
    }
    if (flag & SELECT_WRITE)
    {
        event.events |= EPOLLOUT;
    }

    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, handler->get_descriptor(), &event) != 0)
    {
        return false;
    }

    return true;
}

bool EpollSelector::remove(IHandler* handler)
{
    if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, handler->get_descriptor(), NULL) != 0)
    {
        return false;
    }

    return true;
}

bool EpollSelector::modify(IHandler* handler, int flag)
{
    if (false == remove(handler))
    {
        return false;
    }

    if (false == add(handler, flag))
    {
        return false;
    }

    return true;
}
