#ifndef __MDN_TIMER_WHEEL_H__
#define __MDN_TIMER_WHEEL_H__

#include "mdn_timer_item.h"
#include "mdn_timer.h"

#include <vector>

class TimerWheel
{
public:
    TimerWheel(int size, int milisec);
    ~TimerWheel();

public:
    int get_min_time() const;
    int get_max_time() const;

    int get_item_count() const;

    int get_remain_time() const;
    int get_passed_time() const;

    void place_timer(int place, Timer* timer);

    TimerItem* click(TimerItem* item);

private:
    typedef std::vector<TimerItem*> TimerItemVector;

private:
    TimerItemVector timer_item_vector_;
    int cursor_;
    int time_unit_;
};

#endif  // __MDN_TIMER_WHEEL_H__