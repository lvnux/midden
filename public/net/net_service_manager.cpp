#include "net_service_manager.h"
#include "net_server_socket.h"
#include "epoll_selector.h"
// #include "select_selector.h"
// #include "poll_selector.h"


NetServiceManager::NetServiceManager()
{

}

NetServiceManager::~NetServiceManager()
{

}

bool NetServiceManager::init()
{
    selector_ = new EpollSelector();
    bool ret = selector_->init();
    if (false == ret)
    {
        printf("selector_ init failed\n");
        return false;
    }

    buffer_manager_ = new LinkedBufferManager();
    ret = buffer_manager_->init();
    if (false == ret)
    {
        printf("buffer_manager_ init failed\n");
        return false;
    }

    return true;
}

ISelector* NetServiceManager::get_selector() const
{
    return selector_;
}

LinkedBufferManager* NetServiceManager::get_buffer_manager() const
{
    return buffer_manager_;
}

NetServerSocket* NetServiceManager::create_server_socket(BaseThread* thread)
{
    return new NetServerSocket(this, thread);
}