/*
 * iselector.h
 * 多路复用机制基类
 */

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

        // 0x04 - 0x0f 保留
        
        SELECT_ALL = 0x0f,
    };

public:
    // 初始化
    virtual bool init() = 0;

    // 清理工作
    virtual bool destroy() = 0;

    // 等待事件返回
    virtual int select() = 0;

    // 添加事件监控
    virtual bool add(IHandler* handler, int flag) = 0;

    // 删除事件
    virtual bool remove(IHandler* handler) = 0;

    // 更改事件监听属性
    virtual bool modify(IHandler* handler, int flag) = 0;
};

#endif  // __I_SELECTOR_H__