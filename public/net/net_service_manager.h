/*
 * net_service_manager.h
 * 网络服务管理类
 */

#ifndef __NET_SERVICE_MANAGER_H__
#define __NET_SERVICE_MANAGER_H__

#include "iselector.h"

class NetServerSocket;
class NetServiceManager
{
public:
    NetServiceManager();
    ~NetServiceManager();

public:
    bool init();
    ISelector* get_selector() const;
    NetServerSocket* create_server_socket();

private:
    ISelector* selector_;
};

#endif  // __NET_SERVICE_MANAGER_H__