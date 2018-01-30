/*
 * AutoRef.h
 * 自动引用计数
 */

#ifndef __AUTO_REF_H__
#define __AUTO_REF_H__

class AutoRef
{
public:
    AutoRef()
    {
        ref_ = 0;
    }

    virtual ~AutoRef(){}
    
public:
    // 增加计数
    virtual void add_automic()
    {
        ref_++;
    }

    // 减少计数
    virtual void del_automic()
    {
        ref_--;
    }

    // 获取计数
    virtual int  get_automic()
    {
        return ref_;
    }

private:
    int ref_;
};

#endif  // __AUTO_REF_H__