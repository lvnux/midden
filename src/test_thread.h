#ifndef __TEST_THREAD_H__
#define __TEST_THREAD_H__

#include "net_service_manager.h"
#include "base_thread.h"

class TestThread : public BaseThread
{
public:
    virtual bool init();

protected:
    virtual bool main_loop();
    virtual void dispose(BaseMsg* msg);

private:
    void dispose_accept_socket(BaseMsg* msg);

private:
    NetServiceManager* manager_;
};

#endif  // __TEST_THREAD_H__