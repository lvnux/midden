#include "test_thread.h"
#include "net_server_socket.h"
#include "thread_macro.h"
#include "test_socket_handler.h"


bool TestThread::init()
{
    manager_ = new NetServiceManager();
    if (false == manager_->init())
    {
        printf("manager_->init() failed\n");
        return false;
    }

    NetServerSocket* server = manager_->create_server_socket(this);
    if (false == server->listen(NetAddress("0.0.0.0", 8888)))
    {
        printf("server->listen() failed\n");
        return false;
    }

    block_pool_ = new LinkedBlockPool();

    return true;
}


bool TestThread::main_loop()
{
    BaseMsg* msg = NULL;
    while (true)
    {
        // printf("select\n");
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

        sleep(2);
    }

    return true;
}

void TestThread::dispose(BaseMsg* msg)
{
    if (NULL == msg)
    {
        return;
    }

    printf("disponse: %d, %s\n", msg->msg_type, (msg->msg_id).c_str());

    switch (msg->msg_type)
    {
        case MSG_TYPE_ACCEPT_SOCKET:
        {
            dispose_accept_socket(msg);
            break;
        }
        default:
        {
            printf("msg_type[%d] is invalied.\n", msg->msg_type);
            break;
        }
    }

    return;
}

void TestThread::dispose_accept_socket(BaseMsg* msg)
{
    TAcceptSocketMsg* pMsg = (TAcceptSocketMsg*)msg;
    printf("msg type: [%d]\n", pMsg->msg_type);

    TestSocketHandler* handler = new TestSocketHandler(this);
    if (false == handler->init(manager_, pMsg->socket, pMsg->address, block_pool_))
    {
        printf("TestThread::dispose_accept_socket failed\n");
        delete handler;
        return;
    }
}