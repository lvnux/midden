#ifndef __NET_SERVER_SOCKET_H__
#define __NET_SERVER_SOCKET_H__

#include "ihandler.h"
#include "net_address.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class NetServiceManager;
class NetServerSocket : public IHandler
{
public:
    NetServerSocket(NetServiceManager* manager);
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
};

#endif  // __NET_SERVER_SOCKET_H__