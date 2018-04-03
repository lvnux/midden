#include "mdn_timer_manager.h"

#include <signal.h>
#include <time.h>
#include <string.h>

void on_alarm(union sigval sig)
{
    TimerManager* manager = (TimerManager*)sig.sival_ptr;
    manager->incr_tick();
}

TimerManager::TimerManager()
    : tick_(0), time_unit_(100), count_(3000), timer_item_(new TimerItem())
{

}

TimerManager::~TimerManager()
{
    if (NULL != timer_item_)
    {
        delete timer_item_;
        timer_item_ = NULL;
    }
}


void TimerManager::place_timer(Timer* timer)
{
    int round_time = time_unit_;
    int remain;
    for (TimerWheelList::iterator itor = timer_wheel_list_.begin(); itor != timer_wheel_list_.end(); ++itor)
    {
        remain = timer->get_remain();
        round_time = (*itor)->get_max_time();
        if (remain < round_time)
        {
            (*itor)->place_timer(remain / (*itor)->get_min_time(), timer);
            return;
        }
        else
        {
            timer->set_remain(timer->get_remain() + (*itor)->get_passed_time());  // 以便在遍历到timer所在item的时候，将其设置到time_unit较小的item时，从当前的cursor开始，而不是从0开始
        }
    }

    while (true)
    {
        TimerWheel* wheel = new TimerWheel(count_, round_time);
        timer_wheel_list_.push_back(wheel);

        remain = timer->get_remain();
        round_time = wheel->get_max_time();
        if (remain < round_time)
        {
            wheel->place_timer(remain / wheel->get_min_time(), timer);
            return;
        }
        else
        {
            timer->set_remain(timer->get_remain() + wheel->get_passed_time());
        }
    }
}

bool TimerManager::init()
{
    struct sigevent evp;
    struct itimerspec its;

    timer_t timerid;

    memset(&evp, 0, sizeof(evp));
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = on_alarm;
    evp.sigev_value.sival_ptr = this;

    if (timer_create(CLOCK_REALTIME, &evp, &timerid) != 0)
    {
        printf("timer_create failed\n");
        return false;
    }

    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = time_unit_ * 1000 * 1000;  // 100ms
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = time_unit_ * 1000 * 1000;

    if (timer_settime(timerid, CLOCK_REALTIME, &its, NULL) != 0)
    {
        printf("timer_settime failed\n");
        return false;
    }

    return true;
}

bool TimerManager::destroy()
{
    return true;
}

void TimerManager::click()
{
    int click = get_passed_time();   // 时间过去了多少个单位
    int min_time;

    for (int i = 0; i < click; i++)
    {
        for (TimerWheelList::iterator itor = timer_wheel_list_.begin(); itor != timer_wheel_list_.end(); ++itor)
        {
            min_time = (*itor)->get_min_time();
            timer_item_ = (*itor)->click(timer_item_);  // 调用一次，对应的 cursor_ 移动一个单元
            while (timer_item_->begin() != timer_item_->end())
            {
                Timer* timer = timer_item_->pop();
                timer->set_remain(timer->get_remain() % min_time);  // 一个单元格存储的都是在一个时间单元的事件，比如都是 300-400
                if (timer->get_remain() < time_unit_)
                {
                    timer->on_timer();
                }
                else
                {
                    place_timer(timer);
                }
            }
            timer_item_->reset();

            // cursor_重新归零时，表示一段时间(一个timer_item的时间) 单元时间*个数 过去了，才需要继续遍历后面的结构
            if ((*itor)->get_passed_time() != 0)
            {
                break;
            }
        }
    }
}

Timer* TimerManager::create_timer()
{
    return new Timer(this);
}

void TimerManager::incr_tick()
{
    ++tick_;
}

int TimerManager::get_current_tick()
{
    return tick_;
}

int TimerManager::get_passed_time()
{
    int passed_time = tick_;
    tick_ = 0;
    return passed_time;
}