/*
 * net_socket.h
 * 通用socket
 */

#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

#include "ihandler.h"
#include "iflush_handler.h"
#include "net_service_manager.h"
#include "net_address.h"
#include "inet_socket_handler.h"
#include "event_type.h"
#include "linked_block_pool.h"


class NetSocket : public IHandler, public IFlushHandler
{
public:
    NetSocket();
    ~NetSocket();

public:
    bool init(NetServiceManager* manager, LinkedBlockPool* pool);
    bool init(NetServiceManager* manager, int socket, const NetAddress& address, LinkedBlockPool* pool);
    void connect(const NetAddress& address);
    void close();
    void set_peek(bool flag);
    const char* get_last_error() const;
    
    NetAddress get_local_address();
    NetAddress get_remote_address();

    void set_socket_handler(INetSocketHandler* handler);

    IBufferIn* get_buffer_in() const;
    IBufferOut* get_buffer_out() const;

public:
    virtual int get_descriptor();
    virtual void on_read();
    virtual void on_write();

public:
    virtual void on_flush(IBufferOut* buffer);

public:
    void on_connected();
    void notify_event(EventType type);
    void send();

    bool has_state(uint8 state);

    /*
     设置状态：
     flag为true设置状态；flag为false清除状态。
     */
    bool set_state(uint8 state, bool flag=true);

private:
    // socket的状态
    enum SocketState
    {
        CLOSING = 0x01,
        CONNECTING = 0x02,
        CONNECTED = 0x04,
        WRITABLE = 0x08,
        DELAYED = 0x10
    };

private:
    uint8 state_;
    bool flag_;
    int socket_;

    IBufferIn* buffer_in_;
    IBufferOut* buffer_out_;

    INetSocketHandler* handler_;
    NetServiceManager* manager_;
};

#endif  // __NET_SOCKET_H__