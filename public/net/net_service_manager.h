#ifndef __NET_SERVICE_MANAGER_H__
#define __NET_SERVICE_MANAGER_H__

#include "epoll_selector.h"

class NetServerSocket;
class NetServiceManager
{
public:
    NetServiceManager();
    ~NetServiceManager();

public:
    bool init();
    EpollSelector* get_selector() const;
    NetServerSocket* create_server_socket();

private:
    EpollSelector* selector_;
};

#endif  // __NET_SERVICE_MANAGER_H__