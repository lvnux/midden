/*
 * ihandler.h
 * 事件处理基类
 */


#ifndef __I_HANDLER_H__
#define __I_HANDLER_H__

class IHandler
{
public:
    // 返回文件描述符或者socket
    virtual int get_descriptor() = 0;

    // 由多路复用机制回调，处理可写事件
    virtual void on_read() = 0;

    // 由多路复用机制回调，处理可读事件
    virtual void on_write() = 0;
};

#endif  // __I_HANDLER_H__