#include "test_socket_handler.h"
#include "http_response.h"
#include "http_request.h"
#include "log.h"

#include <string>


TestSocketHandler::TestSocketHandler(BaseThread* thread)
{
    net_socket_ = NULL;
    thread_ = thread;
}

TestSocketHandler::~TestSocketHandler()
{
    if (NULL != net_socket_)
    {
        net_socket_->close();
        delete net_socket_;
    }
}

bool TestSocketHandler::init(NetServiceManager* manager, int socket, const NetAddress& address, LinkedBlockPool* pool)
{
    NetSocket* net_socket = new NetSocket();
    if (false == net_socket->init(manager, socket, address, pool))
    {
        log_error("TestSocketHandler::init failed");
        delete net_socket;
        net_socket = NULL;
        return false;
    }

    client_ipaddress_ = address.get_format_ipaddress();
    net_socket->set_socket_handler(this);
    net_socket_ = net_socket;
    return true;
}

void TestSocketHandler::on_event(EventType type, NetSocket* socket)
{
    // printf("TestSocketHandler::on_event get type: %d\n", type);

    if (EventType::Read == type)
    {
        char data[1024] = { 0 };
        net_socket_->get_buffer_in()->read(data, net_socket_->get_buffer_in()->available());
        log_debug("TestSocketHandler::on_event get data: [%s]", data);

        http::HttpRequest request;
        request.decode(data);
        // printf("TestSocketHandler::on_event method: %s, url: %s, path: %s, version: %s, content: %s\n",
        //     request.get_method().c_str(), request.get_url().c_str(), request.get_path().c_str(),
        //     request.get_version().c_str(), request.get_content().c_str());

        // http::Header headers = request.get_headers();
        // for (http::Header::const_iterator itor = headers.begin(); itor != headers.end(); ++itor)
        // {
        //     printf("request key: %s, value: %s\n", itor->first.c_str(), itor->second.c_str());
        // }

        // std::string response_test_data =
        //     "HTTP/1.1 200 OK\r\n"
        //     "Content-Length:16\r\n"
        //     "Content-Type:text/html; charset=UTF-8\r\n"
        //     "test: hhh eee\r\n"
        //     "\r\n"
        //     "response: helo";

        // http::HttpResponse response_test;
        // response_test.decode(response_test_data);
        // printf("TestSocketHandler::on_event response_test version: %s, status: %d, reason: %s, content: %s\n",
        //     response_test.get_version().c_str(), response_test.get_status(), response_test.get_reason().c_str(), 
        //     response_test.get_content().c_str());

        // headers = response_test.get_headers();
        // for (http::Header::const_iterator itor = headers.begin(); itor != headers.end(); ++itor)
        // {
        //     printf("response key: %s, value: %s\n", itor->first.c_str(), itor->second.c_str());
        // }

        std::string response_content = "get: \r\n";
        response_content.append("path: ").append(request.get_path()).append("\r\n");
        response_content.append("body: ").append(request.get_content()).append("\r\n");

        http::HttpResponse response;
        response.set_status(200);
        response.set_content(response_content);
        std::string content;
        response.encode(content);

        net_socket_->get_buffer_out()->write(content.c_str(), content.size());
    }
}