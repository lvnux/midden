#ifndef __MDN_TIMER_H__
#define __MDN_TIMER_H__

#include "base_thread.h"

class TimerManager;
class TimerItem;
class BaseMsg;

class Timer
{
public:
    Timer(TimerManager* manager);
    virtual ~Timer();

public:
    Timer* get_prev();
    Timer* get_next();
    void set_prev(Timer* prev);
    void set_next(Timer* next);

    void on_timer();
    
    int get_remain() const;
    void set_remain(int remain);

    void on_schedule(TimerItem* item);
    virtual bool schedule(BaseMsg* msg, int milisec);

    virtual void destroy();

private:
    TimerManager* manager_;
    TimerItem* item_;
    int remain_;
    BaseMsg* msg_;
    Timer* prev_;
    Timer* next_;
};

#endif  // __MDN_TIMER_H__