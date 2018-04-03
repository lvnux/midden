#include "test_thread.h"
#include "net_server_socket.h"
#include "thread_macro.h"
#include "test_socket_handler.h"
#include "log.h"


TestThread::TestThread()
    : BaseThread()
{

}

TestThread::~TestThread()
{

}

bool TestThread::init(const std::string& ipaddress, int port)
{
    if (false == BaseThread::init())
    {
        printf("BaseThread::init() failed\n");
        return false;
    }

    manager_ = new NetServiceManager();
    if (false == manager_->init())
    {
        printf("manager_->init() failed\n");
        return false;
    }

    NetServerSocket* server = manager_->create_server_socket(this);
    if (false == server->listen(NetAddress(ipaddress, port)))
    {
        printf("server->listen() failed\n");
        return false;
    }

    block_pool_ = new LinkedBlockPool();

    timer_ = set_timer("test_timer", 3 * 1000);
    if (NULL == timer_)
    {
        printf("set timer failed\n");
        return false;
    }

    return true;
}


bool TestThread::main_loop()
{
    BaseMsg* msg = NULL;
    while (true)
    {
        // printf("select\n");
        timer_manager_->click();
        manager_->get_selector()->select();

        pthread_mutex_lock(&mutex_);
        msg = que_.get_data();
        pthread_mutex_unlock(&mutex_);

        if (NULL == msg)
        {
            usleep(1000 * 10);
        }
        else
        {
            dispose(msg);
            delete msg;
        }
    }

    return true;
}

void TestThread::dispose(BaseMsg* msg)
{
    if (NULL == msg)
    {
        return;
    }

    // printf("disponse: %d, %s\n", msg->msg_type, (msg->msg_id).c_str());

    switch (msg->msg_type)
    {
        case MSG_TYPE_ACCEPT_SOCKET:
        {
            dispose_accept_socket(msg);
            break;
        }
        case MSG_TYPE_HTTP_RESPONSE:
        {
            dispose_http_response(msg);
            break;
        }
        case MSG_TYPE_TIMEOUT:
        {
            log_debug("test timer: %s", msg->msg_id.c_str());
            if (NULL != timer_)
            {
                delete timer_;
                timer_ = NULL;
            }
            timer_ = set_timer("test_timer", 3*1000);
            break;
        }
        default:
        {
            log_error("msg_type[%d] is invalied.", msg->msg_type);
            break;
        }
    }

    return;
}

void TestThread::dispose_accept_socket(BaseMsg* msg)
{
    TAcceptSocketMsg* pMsg = (TAcceptSocketMsg*)msg;
    // printf("msg type: [%d]\n", pMsg->msg_type);

    TestSocketHandler* handler = new TestSocketHandler(this);
    if (false == handler->init(manager_, pMsg->socket, pMsg->address, block_pool_))
    {
        log_error("TestThread::dispose_accept_socket failed");
        delete handler;
        return;
    }

    http_client_.init(manager_, this, block_pool_);
    http_client_.get("http://112.74.216.118:9999/test_hello");
}

void TestThread::dispose_http_response(BaseMsg* msg)
{
    http::THttpResponseMsg* pMsg = (http::THttpResponseMsg*)msg;
    log_error("%d, %s", pMsg->status, pMsg->response.get_content().c_str());
}