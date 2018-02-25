#include "net_socket.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

NetSocket::NetSocket()
    : socket_(-1)
{

}

NetSocket::~NetSocket()
{
    if (-1 != socket_)
    {
        ::close(socket_);
    }
}

bool NetSocket::init(NetServiceManager* manager)
{
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == socket_)
    {
        printf("create socket failed, err: [%s]\n", strerror(errno));
        return false;
    }

    handler_ = NULL;
    state_ = 0;
    flag_ = false;
    manager_ = manager;

    int flags = fcntl(socket_, F_GETFL);
    if (flags == -1)
    {
        printf("fcntl failed, err: [%s]\n", strerror(errno));
        return false;
    }

    flags = fcntl(socket_, F_SETFL, flags |= O_NONBLOCK);
    if (flags == -1)
    {
        printf("fcntl failed, err: [%s]\n", strerror(errno));
        return false;
    }

    int32 on = 1;
    if (setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on)) != 0)
    {
        printf("setsockopt failed, err: [%s]\n", strerror(errno));
        return false;
    }

    if (setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on)) != 0)
    {
        printf("setsockopt failed, err: [%s]\n", strerror(errno));
        return false;
    }

    return manager_->get_selector()->add(this, ISelector::SELECT_ALL);
}

bool NetSocket::init(NetServiceManager* manager, int socket, const NetAddress& address)
{
    socket_ = socket;
    handler_ = NULL;
    state_ = 0;
    flag_ = false;
    manager_ = manager;

    int flags = fcntl(socket_, F_GETFL);
    if (flags == -1)
    {
        printf("fcntl failed, err: [%s]\n", strerror(errno));
        return false;
    }

    flags = fcntl(socket_, F_SETFL, flags |= O_NONBLOCK);
    if (flags == -1)
    {
        printf("fcntl failed, err: [%s]\n", strerror(errno));
        return false;
    }

    int32 on = 1;
    if (setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (char*)&on, sizeof(on)) != 0)
    {
        printf("setsockopt failed, err: [%s]\n", strerror(errno));
        return false;
    }

    if (setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (char*)&on, sizeof(on)) != 0)
    {
        printf("setsockopt failed, err: [%s]\n", strerror(errno));
        return false;
    }

    return manager_->get_selector()->add(this, ISelector::SELECT_ALL);
}

void NetSocket::connect(const NetAddress& address)
{

}

void NetSocket::close()
{
    manager_->get_selector()->remove(this);
}

void NetSocket::set_peek(bool flag)
{
    flag_ = flag;
}

const char* NetSocket::get_last_error() const
{
    return strerror(errno);
}

NetAddress NetSocket::get_local_address()
{
    sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    if (getsockname(socket_, &addr, &addr_len) == 0)
    {
        if (addr.sa_family == AF_INET)
        {
            sockaddr_in *addr_in = reinterpret_cast<sockaddr_in*>(&addr);
            return NetAddress(addr_in->sin_addr.s_addr, ntohs(addr_in->sin_port));
        }
    }

    return NetAddress(0, 0);
}

NetAddress NetSocket::get_remote_address()
{
    sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    if (getpeername(socket_, &addr, &addr_len) == 0)
    {
        if (addr.sa_family == AF_INET)
        {
            sockaddr_in *addr_in = reinterpret_cast<sockaddr_in*>(&addr);
            return NetAddress(addr_in->sin_addr.s_addr, ntohs(addr_in->sin_port));
        }
    }

    return NetAddress(0, 0);
}

void NetSocket::set_socket_handler(INetSocketHandler* handler)
{
    handler_ = handler;
}

int NetSocket::get_descriptor()
{
    return socket_;
}

void NetSocket::on_read()
{
    bool readed = false;
    bool closed = false;

    int flags;
    int bytes;
    char data[4096];

    while (true)
    {
        flags = 0;
        if (flag_)
        {
            flags |= MSG_PEEK;
        }

        bytes = recv(socket_, data, 4096, flags);
        if (bytes > 0)  // 有数据
        {
            readed = true;
            printf("get data: [%s]\n", data);
            if (flag_)
            {
                break;
            }
        }
        else if (bytes == 0)  // 数据读取完毕
        {
            closed = true;
            break;
        }
        else if (bytes == -1)  // recv遇到错误
        {
            if (errno == EAGAIN)
            {
                break;
            }
            else
            {
                printf("on_read failed, err: [%s]\n", strerror(errno));
                notify_event(EventType::Error);
                break;
            }
        }
        else   // recv遇到未知错误
        {
            printf("on_read failed, ret of recv is [%d], err: [%s]\n", bytes, strerror(errno));
        }
    }

    if (readed)
    {
        notify_event(EventType::Read);
    }

    if (closed)
    {
        notify_event(EventType::Closed);
    }
}

void NetSocket::on_write()
{

}

void NetSocket::on_connected()
{

}

void NetSocket::notify_event(EventType type)
{
    if (NULL != handler_)
    {
        handler_->on_event(type, this);
    }
}

void NetSocket::send()
{

}

bool NetSocket::has_state(uint8 state)
{
    return (state_ & state) != 0;
}

bool NetSocket::set_state(uint8 state, bool flag)
{
    if (flag)
    {
        state_ |= state;
    }
    else
    {
        state_ &= ~state;
    }

    return true;
}
