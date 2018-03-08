#include "net_socket.h"
#include "linked_buffer_in.h"
#include "linked_buffer_out.h"

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

bool NetSocket::init(NetServiceManager* manager, LinkedBlockPool* pool)
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

    buffer_in_ = manager->get_buffer_manager()->create_buffer_in(pool);
    buffer_out_ = manager->get_buffer_manager()->create_buffer_out(pool);
    buffer_out_->set_flush_handler(this);

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

bool NetSocket::init(NetServiceManager* manager, int socket, const NetAddress& address, LinkedBlockPool* pool)
{
    socket_ = socket;
    handler_ = NULL;
    state_ = 0;
    flag_ = false;
    manager_ = manager;

    buffer_in_ = manager->get_buffer_manager()->create_buffer_in(pool);
    buffer_out_ = manager->get_buffer_manager()->create_buffer_out(pool);
    buffer_out_->set_flush_handler(this);

    set_state(SocketState::CONNECTING);  // 建立连接中

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
    if (has_state(SocketState::CONNECTING))
    {
        return;
    }

    set_state(SocketState::CONNECTING);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = address.get_ipaddress();
    addr.sin_port = htons(address.get_port());

    ::connect(socket_, (struct sockaddr*)&addr, sizeof(addr));
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

IBufferIn* NetSocket::get_buffer_in() const
{
    return buffer_in_;
}

IBufferOut* NetSocket::get_buffer_out() const
{
    return buffer_out_;
}

void NetSocket::on_flush(IBufferOut* buffer)
{
    if (has_state(SocketState::WRITABLE) && !has_state(SocketState::DELAYED)
        && !has_state(SocketState::CLOSING) && buffer_out_->size() != 0)
    {
        set_state(SocketState::DELAYED);
        send();
    }
}

void NetSocket::on_read()
{
    bool readed = false;
    bool closed = false;

    int flags;
    int bytes;

    while (true)
    {
        flags = 0;
        if (flag_)
        {
            flags |= MSG_PEEK;
        }

        IBlockIn* block = buffer_in_->get_block();
        bytes = recv(socket_, block->data(), block->size(), flags);
        if (bytes > 0)  // 有数据
        {
            block->fill(bytes);
            readed = true;
        }
        block->release();

        if (bytes > 0 && flag_)
        {
            break;
        }
        
        if (bytes <= 0)
        {
            if (bytes == 0)  // 数据读取完毕
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
            else  // recv遇到未知错误
            {
                printf("on_read failed, ret of recv is [%d], err: [%s]\n", bytes, strerror(errno));
            }
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
    if (has_state(SocketState::CLOSING))
    {
        return;
    }

    if (!has_state(SocketState::CONNECTED))
    {
        on_connected();
    }

    if (has_state(SocketState::CONNECTED))
    {
        set_state(SocketState::WRITABLE);
        if (!has_state(SocketState::DELAYED) && buffer_out_->size() != 0)
        {
            set_state(SocketState::DELAYED);
            send();
        }
    }
}

void NetSocket::on_connected()
{
    int optval = 0;
    socklen_t optlen = sizeof(optval);

    getsockopt(socket_, SOL_SOCKET, SO_ERROR, &optval, &optlen);
    if (0 == optval)
    {
        set_state(SocketState::CONNECTED);
    }
    else
    {
        printf("on_connected failed, err: [%s]\n", strerror(errno));
        notify_event(EventType::Error);
    }
}

void NetSocket::send()
{
    set_state(SocketState::DELAYED, false);

    while (has_state(SocketState::WRITABLE) && buffer_out_->size() != 0)
    {
        IBlockOut* block = buffer_out_->get_block();
        int bytes = ::send(socket_, block->data(), block->size(), MSG_NOSIGNAL);
        if (bytes > 0)
        {
            block->consume(bytes);
        }
        block->release();

        // printf("NetSocket::send data: %s, size: %d, buffer_size: %d\n", 
        //     block->data(), block->size(), buffer_out_->size());

        if (-1 == bytes)
        {
            if (errno == EAGAIN)
            {
                set_state(SocketState::WRITABLE, false);
            }
            else if (errno == EPIPE || errno == ECONNRESET)
            {
                printf("send failed, err: [%s]\n", strerror(errno));
                notify_event(EventType::Error);
                set_state(SocketState::WRITABLE, false);
            }
            else
            {
                printf("send failed, err: [%s]\n", strerror(errno));
            }
        }
    }

    if (buffer_out_->size() == 0)
    {
        notify_event(EventType::Writable);
    }
}

void NetSocket::notify_event(EventType type)
{
    if (NULL != handler_)
    {
        handler_->on_event(type, this);
    }
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
