/*
 * select_selector.h
 * select多路复用
 */

#ifndef __SELECT_SELECTOR_H__
#define __SELECT_SELECTOR_H__

#include "iselector.h"

#include <sys/select.h>
#include <map>


class SelectSelector : public ISelector
{
public:
    SelectSelector();
    ~SelectSelector();

public:
    virtual bool init();
    virtual bool destroy();
    virtual int select();
    virtual bool add(IHandler* handler, int flag);
    virtual bool remove(IHandler* handler);
    virtual bool modify(IHandler* handler, int flag);

private:
    std::map<int, IHandler*> fd_handler_;    // fd和handler的对应关系
    fd_set readfds_;
    fd_set writefds_;
    fd_set exceptfds_;
    int maxfd_;  // 值最大的文件描述符或socket
};

#endif  // __SELECT_SELECTOR_H__