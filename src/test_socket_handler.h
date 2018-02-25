#ifndef __TEST_SOCKET_HANDLER_H__
#define __TEST_SOCKET_HANDLER_H__

#include "inet_socket_handler.h"
#include "net_socket.h"
#include "base_thread.h"
#include "net_service_manager.h"
#include "net_address.h"

#include <string>

class TestSocketHandler : public INetSocketHandler
{
public:
    TestSocketHandler(BaseThread* thread);
    ~TestSocketHandler();

public:
    bool init(NetServiceManager* manager, int socket, const NetAddress& address);

public:
    virtual void on_event(EventType type, NetSocket* socket);

private:
    NetSocket* net_socket_;
    BaseThread* thread_;
    std::string client_ipaddress_;
};

#endif  // __TEST_SOCKET_HANDLER_H__