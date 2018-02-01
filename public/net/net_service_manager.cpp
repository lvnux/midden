#include "net_service_manager.h"
#include "net_server_socket.h"
#include "epoll_selector.h"
#include "select_selector.h"
#include "poll_selector.h"


NetServiceManager::NetServiceManager()
{

}

NetServiceManager::~NetServiceManager()
{

}

bool NetServiceManager::init()
{
    selector_ = new EpollSelector();
    // selector_ = new SelectSelector();

    return selector_->init();
}

ISelector* NetServiceManager::get_selector() const
{
    return selector_;
}

NetServerSocket* NetServiceManager::create_server_socket()
{
    return new NetServerSocket(this);
}