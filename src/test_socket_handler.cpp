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

bool TestSocketHandler::init(NetServiceManager* manager, int socket, const NetAddress& address, LinkedBlockPool* pool)
{
    NetSocket* net_socket = new NetSocket();
    if (false == net_socket->init(manager, socket, address, pool))
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
    char data[4] = { 0 };
    net_socket_->get_buffer_in()->read(data, 2);
    printf("TestSocketHandler::on_event get type: %d, get data: [%s]\n", type, data);
}