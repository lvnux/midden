#ifndef __I_HANDLER_H__
#define __I_HANDLER_H__

class IHandler
{
public:
    virtual int get_descriptor() = 0;
    virtual void on_read() = 0;
    virtual void on_write() = 0;
};

#endif  // __I_HANDLER_H__