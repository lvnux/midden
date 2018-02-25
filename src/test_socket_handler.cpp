#include "test_socket_handler.h"

TestSocketHandler::TestSocketHandler(BaseThread* thread)
{
    net_socket_ = NULL;
    thread_ = thread;
}

TestSocketHandler::~TestSocketHandler()
{
    if (NULL != net_socket_)
    {
        net_socket_->close();
        delete net_socket_;
    }
}

bool TestSocketHandler::init(NetServiceManager* manager, int socket, const NetAddress& address)
{
    NetSocket* net_socket = new NetSocket();
    if (false == net_socket->init(manager, socket, address))
    {
        printf("TestSocketHandler::init failed\n");
        delete net_socket;
        return false;
    }

    client_ipaddress_ = address.get_format_ipaddress();
    net_socket->set_socket_handler(this);
    net_socket_ = net_socket;
    return true;
}

void TestSocketHandler::on_event(EventType type, NetSocket* socket)
{
    printf("TestSocketHandler::on_event get type: %d\n", type);
}