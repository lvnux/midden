#include "mdn_timer_item.h"
#include "mdn_timer.h"

TimerItem::TimerItem()
    : first_(NULL)
{

}

TimerItem::~TimerItem()
{
    first_ = NULL;
}

void TimerItem::reset()
{
    first_ = NULL;
}

void TimerItem::schedule_timer(Timer* timer)
{
    timer->on_schedule(this);

    if (NULL == first_)
    {
        timer->set_prev(NULL);
        timer->set_next(NULL);
        first_ = timer;
    }
    else
    {
        timer->set_prev(NULL);
        timer->set_next(first_);
        first_->set_prev(timer);
        first_ = timer;
    }
}

void TimerItem::delete_timer(Timer* timer)
{
    if (timer == first_)
    {
        first_ = timer->get_next();
        if (NULL != first_)
        {
            first_->set_prev(NULL);
        }  
    }
    else
    {
        if (NULL != timer->get_prev())
        {
            timer->get_prev()->set_next(timer->get_next());
        }
        if (NULL != timer->get_next())
        {
            timer->get_next()->set_prev(timer->get_prev());
        }
    }
}

Timer* TimerItem::begin()
{
    return first_;
}

Timer* TimerItem::end()
{
    return NULL;
}

Timer* TimerItem::pop()
{
    Timer* timer = first_;
    if (NULL != first_)
    {
        first_ = first_->get_next();
        if (NULL != first_)
        {
            first_->set_prev(NULL);
        }
    }
    
    timer->set_prev(NULL);
    timer->set_next(NULL);
    return timer;
}