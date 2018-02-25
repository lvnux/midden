/*
 * net_socket.h
 * 通用socket
 */

#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

#include "ihandler.h"
#include "net_service_manager.h"
#include "net_address.h"
#include "inet_socket_handler.h"
#include "event_type.h"

class NetSocket : public IHandler
{
public:
    NetSocket();
    ~NetSocket();

public:
    bool init(NetServiceManager* manager);
    bool init(NetServiceManager* manager, int socket, const NetAddress& address);
    void connect(const NetAddress& address);
    void close();
    void set_peek(bool flag);
    const char* get_last_error() const;
    
    NetAddress get_local_address();
    NetAddress get_remote_address();

    void set_socket_handler(INetSocketHandler* handler);

public:
    virtual int get_descriptor();
    virtual void on_read();
    virtual void on_write();

public:
    void on_connected();
    void notify_event(EventType type);
    void send();

    bool has_state(uint8 state);

    /*
     设置状态：
     flag为true设置状态；flag为false清除状态。
     */
    bool set_state(uint8 state, bool flag = true);

private:
    uint8 state_;
    bool flag_;
    int socket_;
    INetSocketHandler* handler_;
    NetServiceManager* manager_;
};

#endif  // __NET_SOCKET_H__