#include "base_thread.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <errno.h>
#include <list>


BaseQueue::BaseQueue()
{
    msg_size_ = 0;
}

BaseQueue::~BaseQueue()
{

}

int BaseQueue::get_msg_size()
{
    return msg_size_;
}

void BaseQueue::put_data(BaseMsg* msg)
{
    queue_.push(msg);
    msg_size_++;
}

BaseMsg* BaseQueue::get_data()
{
    if (msg_size_ == 0 || queue_.size() == 0)
    {
        return NULL;
    }

    BaseMsg* msg = NULL;
    msg = queue_.front();
    queue_.pop();
    msg_size_--;
    return msg;
}

/*
 BaseThread
 */
BaseThread::BaseThread()
    : timer_manager_(NULL), threadname_("thread") 
{
    pthread_mutex_init(&mutex_, NULL);
}

BaseThread::BaseThread(const std::string& name)
    : timer_manager_(NULL), threadname_(name)
{
    pthread_mutex_init(&mutex_, NULL);
}

BaseThread::~BaseThread()
{
    pthread_mutex_destroy(&mutex_);
}

bool BaseThread::init()
{
    timer_manager_ = new TimerManager();
    if (NULL == timer_manager_)
    {
        printf("timer_manager_ is NULL\n");
        return false;
    }

    if (timer_manager_->init() == false)
    {
        printf("init timer_manager_ failed\n");
        return false;
    }

    return true;
}

bool BaseThread::start()
{
    pthread_attr_t attr;
    memset(&attr, 0, sizeof(pthread_attr_t));
    if (0 != pthread_attr_init(&attr))
    {
        std::cout << "pthread_attr_init failed" << std::endl;
        return false;
    }

    if (joined_ == false)
    {
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }

    if (pthread_create(&thread_, &attr, run, this) != 0)
    {
        std::cout << "create thread failed" <<  std::endl;
        return false;
    }

    int rc = pthread_setname_np(thread_, get_threadname().c_str());
    if (0 != rc)
    {
        std::cout << "set thread name " << get_threadname() << " failed" << std::endl;
    }
    char threadname[16] = { 0 };
    pthread_getname_np(thread_, threadname, 16);
    std::cout << "set thread name " << get_threadname() << " successfully" << std::endl;

    if (0 != pthread_attr_destroy(&attr))
    {
        std::cout << "set attr destroy failed" << std::endl;
    }
    
    return true;
}

void BaseThread::stop()
{
    pthread_cancel(thread_);
}

void BaseThread::join()
{
    pthread_join(thread_, NULL);
}

bool BaseThread::is_alive()
{
    int rc = pthread_kill(thread_, 0);
    if (rc == ESRCH || rc == EINVAL)
    {
        return false;
    }

    return true;
}

void BaseThread::put_data(BaseMsg* msg)
{
    pthread_mutex_lock(&mutex_);
    que_.put_data(msg);
    pthread_mutex_unlock(&mutex_);
}

int BaseThread::get_msg_size()
{
    pthread_mutex_lock(&mutex_);
    int msg_size = que_.get_msg_size();
    pthread_mutex_unlock(&mutex_);
    return msg_size;
}

std::string BaseThread::get_threadname()
{
    return threadname_;
}

void* BaseThread::run(void* arg)
{
    BaseThread* ptr = (BaseThread*)arg;
    while (ptr->is_alive())
    {
        if (false == ptr->main_loop())
            break;
    }
    
    return NULL;
}

bool BaseThread::main_loop()
{
    BaseMsg* msg = NULL;
    while (true)
    {
        timer_manager_->click();
        pthread_mutex_lock(&mutex_);
        msg = que_.get_data();
        pthread_mutex_unlock(&mutex_);

        if (NULL == msg)
        {
            usleep(1000 * 10);
        }
        else
        {
            dispose(msg);
            delete msg;
        }
    }
    return true;
}

void BaseThread::dispose(BaseMsg* msg)
{
    return;
}

Timer* BaseThread::set_timer(std::string msg_id, int milisec)
{
    BaseMsg* msg = new BaseMsg();
    msg->msg_id = msg_id;
    msg->msg_type = MSG_TYPE_TIMEOUT;
    msg->sender = this;

    Timer* timer = timer_manager_->create_timer();
    timer->schedule(msg, milisec);

    return timer;
}