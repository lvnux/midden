#ifndef __INET_SOCKET_HANDLER_H__
#define __INET_SOCKET_HANDLER_H__

#include "event_type.h"

class NetSocket;
class INetSocketHandler
{
public:
    virtual void on_event(EventType type, NetSocket* socket) = 0;
};

#endif  // __INET_SOCKET_HANDLER_H__