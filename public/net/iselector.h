#ifndef __I_SELECTOR_H__
#define __I_SELECTOR_H__

#include "ihandler.h"


class ISelector
{
public:
    enum SelectFlag
    {
        SELECT_READ = 0x01,
        SELECT_WRITE = 0x02,
        SELECT_EXCEPT = 0x04,

        // 0x04 - 0x0f is reserved
        
        SELECT_ALL = 0x0f,
    };

public:
    virtual bool init() = 0;
    virtual bool destroy() = 0;
    virtual int select() = 0;
    virtual bool add(IHandler* handler, int flag) = 0;
    virtual bool remove(IHandler* handler) = 0;
    virtual bool modify(IHandler* handler, int flag) = 0;
};

#endif  // __I_SELECTOR_H__