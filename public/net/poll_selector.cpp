#include "poll_selector.h"

#include <string.h>


PollSelector::PollSelector()
    : max_index_(-1)
{

}

PollSelector::~PollSelector()
{

}

bool PollSelector::init()
{
    // 初始化pollfds_中所有fd为负值
    for (int i = 0; i < OPEN_MAX; i++)
    {
        pollfds_[i].fd = -1;
    }

    return true;
}

bool PollSelector::destroy()
{
    return true;
}

int PollSelector::select()
{
    int count = poll(pollfds_, max_index_+1, 0);
    for (int i = 0; i <= max_index_; i++)
    {
        if (pollfds_[i].fd < 0)
        {
            // 无效fd
            continue;
        }

        if (pollfds_[i].revents & POLLIN)
        {
            fd_handler_[pollfds_[i].fd]->on_read();
        }
        if (pollfds_[i].revents & POLLOUT)
        {
            fd_handler_[pollfds_[i].fd]->on_write();
        }
    }

    return count;
}

bool PollSelector::add(IHandler* handler, int flag)
{
    int i = 0;
    for (; i < OPEN_MAX; i++)
    {
        if (pollfds_[i].fd < 0)
        {
            // 空闲位
            break;
        }
    }

    if (i >= OPEN_MAX)
    {
        // 已超出poll处理能力
        return false;
    }

    pollfds_[i].fd = handler->get_descriptor();

    if ((flag & SELECT_READ) && (flag & SELECT_WRITE))
    {
        pollfds_[i].events = POLLIN | POLLOUT;
    }
    else if (flag & SELECT_READ)
    {
        pollfds_[i].events = POLLIN;
    }
    else if (flag & SELECT_READ)
    {
        pollfds_[i].events = POLLOUT;
    }
    
    if (max_index_ < i)
    {
        max_index_ = i;
    }

    fd_handler_[pollfds_[i].fd] = handler;

    return true;
}

bool PollSelector::remove(IHandler* handler)
{
    int fd = handler->get_descriptor();
    for (int i = 0; i <= max_index_; i++)
    {
        if (pollfds_[i].fd == fd)
        {
            pollfds_[i].fd = -1;
            break;
        }
    }
    fd_handler_.erase(fd);
    return true;
}

bool PollSelector::modify(IHandler* handler, int flag)
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
