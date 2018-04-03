#ifndef __MDN_TIMER_MANAGER_H__
#define __MDN_TIMER_MANAGER_H__

#include "mdn_timer.h"
#include "mdn_timer_item.h"
#include "mdn_timer_wheel.h"

#include <list>

class TimerWheel;
class TimerManager
{
public:
    TimerManager();
    virtual ~TimerManager();

public:
    void place_timer(Timer* timer);

    virtual bool init();
    virtual bool destroy();
    virtual void click();
    virtual Timer* create_timer();

    void incr_tick();

private:
    typedef std::list<TimerWheel*> TimerWheelList;

private:
    int get_current_tick();
    int get_passed_time();

private:
    int tick_;
    TimerWheelList timer_wheel_list_;
    int time_unit_;
    int count_;
    TimerItem* timer_item_;
};

#endif  // __MDN_TIMER_MANAGER_H__