/*
 * http_client.h
 * http客户端
 */

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "inet_socket_handler.h"
#include "net_service_manager.h"
#include "net_socket.h"
#include "base_thread.h"
#include "http_request.h"
#include "http_response.h"
#include "linked_block_pool.h"

#include <string>


namespace http
{
    class THttpResponseMsg : public BaseMsg
    {
    public:
        enum
        {
            HTTP_RESPONSE,
            HTTP_TIMEOUT,
            HTTP_CLOSED,
            HTTP_ERR,
        };
        HttpResponse response;
        uint32 status;
    };

    class HttpClient : public INetSocketHandler
    {
    public:
        HttpClient();
        virtual ~HttpClient();

    public:
        bool init(NetServiceManager* manager, BaseThread* thread, LinkedBlockPool* pool);
        void set_header(const std::string& key, const std::string& value);
        void set_header(const std::string& key, int value); 
        void get(const std::string& url);
        void post(const std::string& url, const std::string& body);
        void destroy();

    protected:
        virtual void on_event(EventType type, NetSocket* socket);

    private:
        void send(const std::string& data);
        bool on_data(const char* data, uint32 size);

    private:
        NetSocket* socket_;
        BaseThread* thread_;
        HttpRequest request_;
    };
}

#endif  // __HTTP_CLIENT_H__