#include "net_service_manager.h"
#include "net_server_socket.h"

NetServiceManager::NetServiceManager()
{

}

NetServiceManager::~NetServiceManager()
{

}

bool NetServiceManager::init()
{
    selector_ = new EpollSelector();

    return selector_->init();
}

EpollSelector* NetServiceManager::get_selector() const
{
    return selector_;
}

NetServerSocket* NetServiceManager::create_server_socket()
{
    return new NetServerSocket(this);
}