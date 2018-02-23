#include "log.h"
#include "net_server_socket.h"
#include "net_service_manager.h"
#include "BaseThread.h"

#include <stdio.h>
#include <unistd.h>


class TestThread : public BaseThread
{
public:
    virtual bool init()
    {
        manager_ = new NetServiceManager();
        if (false == manager_->init())
        {
            printf("manager_->init() failed\n");
            return false;
        }

        NetServerSocket* server = manager_->create_server_socket(this);
        if (false == server->listen(NetAddress(8888)))
        {
            printf("server->listen() failed\n");
            return false;
        }

        return true;
    }

protected:
    virtual bool main_loop()
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

    virtual void dispose(BaseMsg* msg)
    {
        printf("disponse: %d, %s\n", msg->msg_type, (msg->msg_id).c_str());
        return;
    }

private:
    NetServiceManager* manager_;
};


int main(int argc, char* argv[])
{
    TestThread* test = new TestThread();
    test->init();
    test->start();

    while (true)
    {
        sleep(2);
    }

    return 0;
}