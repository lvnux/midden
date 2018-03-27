#ifndef __TEST_THREAD_H__
#define __TEST_THREAD_H__

#include "net_service_manager.h"
#include "base_thread.h"
#include "linked_block_pool.h"
#include "http_client.h"


class TestThread : public BaseThread
{
public:
    virtual bool init();

protected:
    virtual bool main_loop();
    virtual void dispose(BaseMsg* msg);

private:
    void dispose_accept_socket(BaseMsg* msg);
    void dispose_http_response(BaseMsg* msg);

private:
    NetServiceManager* manager_;
    LinkedBlockPool* block_pool_;

private:
    http::HttpClient http_client_;  // 测试用
};

#endif  // __TEST_THREAD_H__