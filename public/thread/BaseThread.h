/*
 * BaseThread.h
 * 线程基类
 */

#ifndef __BASETHREAD_H__
#define __BASETHREAD_H__

#include <pthread.h>
#include <string>
#include <queue>


// 消息体，用于线程间传递
class BaseThread;
class BaseMsg
{
public:
    BaseMsg()
    {
        msg_type = 0;
        msg_id = "";
        sender = NULL;
    }

    virtual ~BaseMsg(){}

public:
    int msg_type;
    std::string msg_id;
    BaseThread* sender;
};


// 消息队列，每个线程拥有各自的消息队列
class BaseQueue
{
public:
    BaseQueue();
    ~BaseQueue();

public:
    int get_msg_size();
    void put_data(BaseMsg* msg);
    BaseMsg* get_data();

public:
    int msg_size_;
    std::queue<BaseMsg*> queue_;
};


class BaseThread
{
public:
    BaseThread();

    BaseThread(const std::string& name);

    virtual ~BaseThread();

public:
    // 初始化工作
    virtual bool init();

    // 启动线程
    bool start();

    // 结束线程
    void stop();

    // 等待线程结束
    void join();

    std::string get_threadname();

    bool is_alive();

    void put_data(BaseMsg* msg);
    int get_msg_size();

protected:
    // 回调执行业务逻辑
    static void* run(void* arg);

    // 回调执行业务逻辑
    virtual bool main_loop();

    // 处理消息
    virtual void dispose(BaseMsg* msg);

protected:
    BaseQueue que_;
    pthread_mutex_t mutex_;

private:
    bool joined_;
    pthread_t thread_;
    std::string threadname_;
};

#endif //__BASETHREAD_H__
