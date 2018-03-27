#include "http_client.h"
#include "net_address.h"
#include "general.h"
#include "thread_macro.h"

#include <string.h>


namespace http
{
    HttpClient::HttpClient()
    : socket_(NULL), thread_(NULL)
    {

    }

    HttpClient::~HttpClient()
    {
        if (NULL != socket_)
        {
            socket_->close();
            delete socket_;
        }
    }

    bool HttpClient::init(NetServiceManager* manager, BaseThread* thread, LinkedBlockPool* pool)
    {
        thread_ = thread;
        NetSocket* net_socket = new NetSocket();
        if (false == net_socket->init(manager, pool))
        {
            printf("HttpClient::init failed\n");
            delete net_socket;
            net_socket = NULL;
            return false;
        }

        net_socket->set_socket_handler(this);
        socket_ = net_socket;
        return true;
    }

    void HttpClient::set_header(const std::string& key, const std::string& value)
    {
        request_.set_header(key, value);
    }

    void HttpClient::set_header(const std::string& key, int value)
    {
        request_.set_header(key, value);
    }

    void HttpClient::get(const std::string& url)
    {
        request_.set_method("GET");
        request_.set_url(url);
        std::string data;
        request_.encode(data);
        
        send(data);
    }

    void HttpClient::post(const std::string& url, const std::string& body)
    {
        request_.set_method("GET");
        request_.set_url(url);
        request_.set_content(body);
        std::string data;
        request_.encode(data);
        
        send(data);
    }

    void HttpClient::destroy()
    {
        socket_->close();
        delete socket_;
    }

    void HttpClient::on_event(EventType type, NetSocket* socket)
    {
        if (type == EventType::Connected)
        {
            ;
        }
        else if (type == EventType::Read)
        {
            uint32 size = socket_->get_buffer_in()->available();
            char* buffer = new char[size];
            memset(buffer, 0, size);
            if (socket_->get_buffer_in()->read(buffer, size))
            {
                if (on_data(buffer, size))
                {
                    socket_->get_buffer_in()->mark();
                }
                else
                {
                    socket_->get_buffer_in()->reset();
                }
            }
            delete []buffer;
        }
        else if (type == EventType::Closed)
        {
            ;
        }
        else if (type == EventType::Error)
        {
            ;
        }
        else
        {
            ;
        }
    }

    void HttpClient::send(const std::string& data)
    {
        NetAddress address;
        std::string host = request_.get_host();
        std::string::size_type pos = host.find(":");
        if (pos != std::string::npos)
        {
            address.set_ipaddress(host.substr(0, pos));
            address.set_port(atoi(host.substr(pos+1).c_str()));
        }
        else
        {
            address.set_ipaddress(host);
            address.set_port(80);
        }

        socket_->connect(address);
        socket_->get_buffer_out()->write(data.c_str(), data.size());
        socket_->get_buffer_out()->flush();
    }

    bool HttpClient::on_data(const char* data, uint32 size)
    {
        std::string response_data = std::string(data, size);

        uint32 status = THttpResponseMsg::HTTP_ERR;
        bool is_completed = false;
        THttpResponseMsg* msg = new THttpResponseMsg();
        if (!msg->response.decode(response_data))
        {
            printf("HttpClient::on_data decode [%s] error\n", response_data.c_str());
            status = THttpResponseMsg::HTTP_ERR;
            is_completed = true;
        }
        else
        {
            status = THttpResponseMsg::HTTP_RESPONSE;
            is_completed = true;
            int content_length;
            if (msg->response.get_header("Content-Length", content_length))
            {
                if ((int)msg->response.get_content().size() < content_length)
                {
                    printf("get header content_length: %d, real content length: %d\n", content_length, (int)msg->response.get_content().size());
                    is_completed = false;
                    delete msg;
                    msg = NULL;
                    return false;
                }
                else
                {
                    is_completed = true;
                }
            }
        }

        std::string chunked;
        if (msg->response.get_header("Transfer-Encoding", chunked) && (to_lower(chunked) == "chunked"))
        {
            // chunked 协议格式：十六进制长度\r\n数据块\r\n十六进制长度\r\n数据块\r\n...0\r\n\r\n
            std::string origin_content = msg->response.get_content();  // 原始数据
            std::string real_content;  // 实际有效数据
            std::string str_length;  // 单个数据块长度本身（十六进制表示的字符串）
            int chunked_length;  // 单个数据块长度本身

            if (origin_content.substr(origin_content.size() - 4) != "\r\n\r\n")
            {
                is_completed = false;
                delete msg;
                msg = NULL;
                return false;
            }
            else
            {
                is_completed = true;
            }

            std::string::size_type pos = origin_content.find_first_of("\r\n");
            while (pos != std::string::npos)
            {
                str_length = origin_content.substr(0, pos);
                chunked_length = hex2int(str_length.c_str(), str_length.size());

                if (origin_content.size() > (pos + 2 + chunked_length + 4))
                {
                    real_content.append(origin_content.substr(pos + 2, chunked_length));
                    origin_content = origin_content.substr(pos + 2 + chunked_length + 2);
                }
                else  // 数据格式错误
                {
                    real_content.append(origin_content.substr(pos + 2));
                    break;
                }
                
                pos = origin_content.find_first_of("\r\n");
            }
        }

        if (true == is_completed)
        {
            msg->status = status;
            msg->msg_type = MSG_TYPE_HTTP_RESPONSE;
            thread_->put_data(msg);
            return true;
        }
        else
        {
            delete msg;
            msg = NULL;
            return false;
        }
    }
}