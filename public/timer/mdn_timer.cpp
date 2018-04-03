#include "mdn_timer.h"
#include "mdn_timer_item.h"
#include "mdn_timer_manager.h"


Timer::Timer(TimerManager* manager)
    : manager_(manager), item_(NULL), remain_(0), msg_(NULL), prev_(NULL), next_(NULL)
{

}

Timer::~Timer()
{
    if (NULL != item_)
    {
        item_->delete_timer(this);
    }

    if (NULL != msg_)
    {
        delete msg_;
        msg_ = NULL;
    }
}


Timer* Timer::get_prev()
{
    return prev_;
}

Timer* Timer::get_next()
{
    return next_;
}

void Timer::set_prev(Timer* prev)
{
    prev_ = prev;
}

void Timer::set_next(Timer* next)
{
    next_ = next;
}

void Timer::on_timer()
{
    if (NULL != msg_ && NULL != msg_->sender)
    {
        msg_->sender->put_data(msg_);
    }
    msg_ = NULL;
}

int Timer::get_remain() const
{
    return remain_;
}

void Timer::set_remain(int remain)
{
    remain_ = remain;
}

void Timer::on_schedule(TimerItem* item)
{
    item_ = item;
}

bool Timer::schedule(BaseMsg* msg, int milisec)
{
    msg_ = msg;
    remain_ = milisec;
    manager_->place_timer(this);
    
    return true;
}

void Timer::destroy()
{
    delete this;
}