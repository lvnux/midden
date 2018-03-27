#include "net_server_socket.h"
#include "net_service_manager.h"
#include "thread_macro.h"

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


NetServerSocket::NetServerSocket(NetServiceManager* manager, BaseThread* thread)
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    manager_ = manager;
    thread_ = thread;
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

    manager_->get_selector()->add(this, ISelector::SELECT_READ);
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
    while (true)
    {
        sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        int socket = accept(socket_, (struct sockaddr*)&addr, &addr_len);
        if (-1 == socket)
        {
            if (errno == EAGAIN)
            {
                break;
            }
            printf("accept error, err: [%s]\n", strerror(errno));
            continue;
        }
        else
        {
            printf("accept socket ip: [%s]\n", inet_ntoa(addr.sin_addr));
            NetAddress address(addr.sin_addr.s_addr, ntohs(addr.sin_port));
            TAcceptSocketMsg* msg = new TAcceptSocketMsg();
            msg->msg_type = MSG_TYPE_ACCEPT_SOCKET;
            msg->socket = socket;
            msg->address = address;
            thread_->put_data(msg);
        }
    }
}

void NetServerSocket::on_write()
{
    printf("on_write\n");
}