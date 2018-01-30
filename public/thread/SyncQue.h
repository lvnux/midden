/*
 * SyncQue.h
 * 线程安全共享队列
 */

#ifndef __SYNC_QUE_H__
#define __SYNC_QUE_H__

#include <list>

template <typename T>
class SyncQue
{
public:
    SyncQue()
    {
        pthread_mutex_init(&mutex_, NULL);
        pthread_cond_init(&cond_, NULL);
    }

    ~SyncQue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }

public:
    void put_data(const T& msg)
    {
        pthread_mutex_lock(&mutex_);
        syncQue_.push_back(msg);
        pthread_mutex_unlock(&mutex_);
        pthread_cond_signal(&cond_);
    }

    T get_data(int seconds=-1)
    {
        T* p = NULL;
        pthread_mutex_lock(&mutex_);
        if (false == syncQue_.empty())
        {
            T item = syncQue_.front();
            syncQue_.pop_front();
            p = &item;
        }
        pthread_mutex_unlock(&mutex_);

        while (NULL == p)
        {
            pthread_mutex_lock(&mutex_);
            pthread_cond_wait(&cond_, &mutex_);
            if (false == syncQue_.empty())
            {
                T item = syncQue_.front();
                syncQue_.pop_front();
                p = &item;
            }
            pthread_mutex_unlock(&mutex_);
        }

        return *p;
    }

    int get_datas(std::list<T>& datas, int max, int seconds=30)
    {
        int* p = NULL;
        pthread_mutex_lock(&mutex_);
        if (false == syncQue_.empty())
        {
            int i = 0;
            for (; i < max && false == syncQue_.empty(); i++)
            {
                T item = syncQue_.front();
                syncQue_.pop_front();
                datas.push_back(item);
            }
            p = &i;
        }
        pthread_mutex_unlock(&mutex_);

        while (NULL == p)
        {
            pthread_mutex_lock(&mutex_);
            pthread_cond_wait(&cond_, &mutex_);
            if (false == syncQue_.empty())
            {
                int i = 0;
                for (; i < max && false == syncQue_.empty(); i++)
                {
                    T item = syncQue_.front();
                    syncQue_.pop_front();
                    datas.push_back(item);
                }
                p = &i;
            }
            pthread_mutex_unlock(&mutex_);
        }

        return *p;
    }

private:
    std::list<T> syncQue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;

};

#endif  //  __SYNC_QUE_H__