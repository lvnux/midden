#include "mdn_timer_wheel.h"


TimerWheel::TimerWheel(int size, int milisec)
{
    cursor_ = 0;
    time_unit_ = milisec;

    timer_item_vector_.resize(size);
    for (int i = 0; i < size; i++)
    {
        timer_item_vector_[i] = new TimerItem();
    }
}

TimerWheel::~TimerWheel()
{

}

int TimerWheel::get_min_time() const
{
    return time_unit_;
}

int TimerWheel::get_max_time() const
{
    return time_unit_ * timer_item_vector_.size();
}

int TimerWheel::get_item_count() const
{
    return timer_item_vector_.size();
}

int TimerWheel::get_remain_time() const
{
    return time_unit_ * (timer_item_vector_.size() - cursor_);
}

int TimerWheel::get_passed_time() const
{
    return time_unit_ * cursor_;
}

void TimerWheel::place_timer(int place, Timer* timer)
{
    /*
     假设单位时间是100
     数据存放从 cursor_ 的下一位开始，cursor_+1位存放 0-100和100-200的，cursor_+2存放200-300的。
     后面取数据的时候，时间过去100了，会把0-100和100-200的回调触发；时间过去300了，就回把300-400的回调触发
     */
    if (place % timer_item_vector_.size() == 0)
    {
        place = 1;
    }

    int pos = (cursor_ + place) % timer_item_vector_.size();
    timer_item_vector_[pos]->schedule_timer(timer);
}

TimerItem* TimerWheel::click(TimerItem* item)
{
    /*每次取数据时，说明时间过去了100，m_iCursor向后移动一位*/
    cursor_ = (cursor_ + 1) % timer_item_vector_.size();
    TimerItem* old = timer_item_vector_[cursor_];
    timer_item_vector_[cursor_] = item;
    
    return old;
}