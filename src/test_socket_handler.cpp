#include "test_socket_handler.h"
#include <string>


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
    printf("TestSocketHandler::on_event get type: %d\n", type);

    if (EventType::Read == type)
    {
        char data[1024] = { 0 };
        net_socket_->get_buffer_in()->read(data, net_socket_->get_buffer_in()->available());
        printf("TestSocketHandler::on_event get data: [%s]\n", data);

        std::string response = "response: ";
        response.append(data);
        net_socket_->get_buffer_out()->write(response.c_str(), response.size());
    }
}