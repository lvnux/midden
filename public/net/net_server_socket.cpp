#include "net_server_socket.h"
#include "net_service_manager.h"

#include <fcntl.h>
#include <string.h>

NetServerSocket::NetServerSocket(NetServiceManager* manager)
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    manager_ = manager;
}

NetServerSocket::~NetServerSocket()
{

}

bool NetServerSocket::listen(const NetAddress& address, int backlog)
{
    int flags = fcntl(socket_, F_GETFL);
    if (-1 == flags)
    {
        return false;
    }

    flags = fcntl(socket_, F_SETFL, flags |= O_NONBLOCK);
    if (-1 == flags)
    {
        return false;
    }

    int reuse = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0)
    {
        return false;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family  = AF_INET;
    addr.sin_addr.s_addr = address.get_ipaddress();
    addr.sin_port = htons(address.get_port());

    if (bind(socket_, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        return false;
    }

    if (::listen(socket_, backlog) == -1)
    {
        return false;
    }

    manager_->get_selector()->add(this, EpollSelector::SELECT_READ);
    return true;
}

void NetServerSocket::close()
{
    manager_->get_selector()->remove(this);
    ::close(socket_);
}

int NetServerSocket::get_descriptor()
{
    return socket_;
}

void NetServerSocket::on_read()
{
    printf("on_read\n");
}

void NetServerSocket::on_write()
{
    printf("on_write\n");
}