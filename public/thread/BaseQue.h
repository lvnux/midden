/*
 * BaseQue.h
 * 消息队列模板类
 */

#ifndef __BASE_QUE_H__
#define __BASE_QUE_H__

#include "SyncQue.h"

template <typename T>
class BaseQue
{
public:
    BaseQue(){}
    virtual ~BaseQue(){}

public:
    void put_data(const T& msg)
    {
        syncQue_.put_data(msg);
    }

    T get_data(int seconds = -1)
    {
        return syncQue_.get_data(seconds);
    }

    int get_datas(std::list<T>& datas, int max, int seconds=30)
    {
        return syncQue_.get_datas(datas, max, seconds);
    }

private:
    SyncQue<T> syncQue_;
};

#endif  //  __BASE_QUE_H__