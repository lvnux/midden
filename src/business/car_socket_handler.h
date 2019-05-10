#ifndef __CAR_SOCKET_HANDLER_H__
#define __CAR_SOCKET_HANDLER_H__

#include "inet_socket_handler.h"
#include "net_socket.h"
#include "base_thread.h"
#include "net_service_manager.h"
#include "net_address.h"
#include "linked_block_pool.h"

#include <string>

class CarSocketHandler : public INetSocketHandler
{
public:
    CarSocketHandler(BaseThread* thread, uint64 sn);
    ~CarSocketHandler();

public:
    bool init(NetServiceManager* manager, int socket, const NetAddress& address, LinkedBlockPool* pool);

public:
    virtual void on_event(EventType type, NetSocket* socket);
	void send_response(int code, const std::string& desc, const std::string& plate_num);

private:
	bool on_data(NetSocket* socket, const char* data, int32 length);
	
private:
	uint64 sn_;
    NetSocket* net_socket_;
    BaseThread* thread_;
    std::string client_ipaddress_;
};

#endif  // __CAR_SOCKET_HANDLER_H__