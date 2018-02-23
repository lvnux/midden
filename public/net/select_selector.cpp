#include "select_selector.h"

#include <string.h>


SelectSelector::SelectSelector()
    : maxfd_(-1)
{

}

SelectSelector::~SelectSelector()
{

}

bool SelectSelector::init()
{
    FD_ZERO(&readfds_);
    FD_ZERO(&writefds_);
    FD_ZERO(&exceptfds_);

    return true;
}

bool SelectSelector::destroy()
{
    return true;
}

int SelectSelector::select()
{
    fd_set readfds, writefds, exceptfds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    memcpy(&readfds, &readfds_, sizeof(readfds_));
    memcpy(&writefds, &writefds_, sizeof(writefds_));
    memcpy(&exceptfds, &exceptfds_, sizeof(exceptfds_));

    int count = ::select(maxfd_+1, &readfds, &writefds, &exceptfds, 0);
    for (std::map<int, IHandler*>::iterator itor = fd_handler_.begin(); itor != fd_handler_.end(); ++itor)
    {
        if (FD_ISSET(itor->first, &readfds))
        {
            itor->second->on_read();
        }
        if (FD_ISSET(itor->first, &writefds))
        {
            itor->second->on_write();
        }
        if (FD_ISSET(itor->first, &exceptfds))
        {
            ;
        }
    }

    return count;
}

bool SelectSelector::add(IHandler* handler, int flag)
{
    int fd = handler->get_descriptor();
    if (flag & SELECT_READ)
    {
        FD_SET(fd, &readfds_);
    }
    if (flag & SELECT_WRITE)
    {
        FD_SET(fd, &writefds_);
    }
    if (flag & SELECT_EXCEPT)
    {
        FD_SET(fd, &exceptfds_);
    }

    if (fd > maxfd_)
    {
        maxfd_ = fd;
    }

    fd_handler_[fd] = handler;

    return true;
}

bool SelectSelector::remove(IHandler* handler)
{
    int fd = handler->get_descriptor();
    if (FD_ISSET(fd, &readfds_))
    {
        FD_CLR(fd, &readfds_);
    }
    if (FD_ISSET(fd, &writefds_))
    {
        FD_CLR(fd, &writefds_);
    }
    if (FD_ISSET(fd, &exceptfds_))
    {
        FD_CLR(fd, &exceptfds_);
    }

    fd_handler_.erase(fd);
    return true;
}

bool SelectSelector::modify(IHandler* handler, int flag)
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
