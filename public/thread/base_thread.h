/*
 * base_thread.h
 * 线程基类
 */

#ifndef __BASETHREAD_H__
#define __BASETHREAD_H__

#include "platform.h"
#include "thread_macro.h"
#include "mdn_timer_manager.h"

#include <pthread.h>
#include <string>
#include <queue>


class BaseThread;

// 消息体，用于线程间传递
class BaseMsg
{
public:
    BaseMsg()
    {
        msg_type = 0;
		msg_sn = 0;
        msg_session_id = "";
        sender = NULL;
		receive_time = 0;
    }

    virtual ~BaseMsg(){}

public:
    int msg_type;
	uint64 msg_sn;
    std::string msg_session_id;
    BaseThread* sender;
	int64 receive_time;
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


class Timer;
class TimerManager;
// 线程基类
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

    Timer* set_timer(uint64 msg_sn, std::string msg_session_id, int milisec);

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
    TimerManager* timer_manager_;

private:
    bool joined_;
    pthread_t thread_;
    std::string threadname_;
};

#endif //__BASETHREAD_H__
