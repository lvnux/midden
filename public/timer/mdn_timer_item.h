#ifndef __MDN_TIMER_ITERM_H__
#define __MDN_TIMER_ITERM_H__

#include <stdio.h>

class Timer;

class TimerItem
{
public:
    TimerItem();
    ~TimerItem();

public:
    void reset();
    void schedule_timer(Timer* timer);
    void delete_timer(Timer* timer);

    Timer* begin();
    Timer* end();
    Timer* pop();

private:
    Timer* first_;
};

#endif  // __MDN_TIMER_ITERM_H__