/*
 * net_server_socket.h
 * 服务端socket
 */

#ifndef __NET_SERVER_SOCKET_H__
#define __NET_SERVER_SOCKET_H__

#include "ihandler.h"
#include "net_address.h"
#include "base_thread.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class TAcceptSocketMsg : public BaseMsg
{
public:
    int socket;
    NetAddress address;
};

class NetServiceManager;
class NetServerSocket : public IHandler
{
public:
    NetServerSocket(NetServiceManager* manager, BaseThread* thread);
    ~NetServerSocket();

public:
    bool listen(const NetAddress& address, int backlog=128);
    void close();

protected:
    virtual int get_descriptor();
    virtual void on_read();
    virtual void on_write();

private:
    int socket_;
    NetServiceManager* manager_;
    BaseThread* thread_;
};

#endif  // __NET_SERVER_SOCKET_H__