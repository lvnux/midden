#include "car_socket_handler.h"
#include "http_response.h"
#include "http_request.h"
#include "user_thread_macro.h"

#include "log.h"
// #include "base_64.h"
#include "json/writer.h"
#include "car_http_thread.h"
#include "general.h"

#include <string>

CarSocketHandler::CarSocketHandler(BaseThread* thread, uint64 sn)
{
	sn_ = sn;
    net_socket_ = NULL;
    thread_ = thread;
}

CarSocketHandler::~CarSocketHandler()
{
    if (NULL != net_socket_)
    {
        net_socket_->close();
        delete net_socket_;
    }
}

bool CarSocketHandler::init(NetServiceManager* manager, int socket, const NetAddress& address, LinkedBlockPool* pool)
{
    NetSocket* net_socket = new NetSocket();
    if (false == net_socket->init(manager, socket, address, pool))
    {
        log_error("CarSocketHandler::init failed");
        delete net_socket;
        net_socket = NULL;
        return false;
    }

    client_ipaddress_ = address.get_format_ipaddress();
    net_socket->set_socket_handler(this);
    net_socket_ = net_socket;
    return true;
}

void CarSocketHandler::on_event(EventType type, NetSocket* socket)
{
    // printf("TestSocketHandler::on_event get type: %d\n", type);

    if (EventType::Read == type)
    {
    	log_debug("CarSocketHandler::on_event read");

		uint32 length = socket->get_buffer_in()->available();
        char* buffer = new char[length];
		
        if (socket->get_buffer_in()->read(buffer, length))
        {
        	if (on_data(socket, buffer, length))
        	{
        		socket->get_buffer_in()->mark();
        	}
			else
			{
				socket->get_buffer_in()->reset();
			}

			delete[] buffer;
        }
    }
}

bool CarSocketHandler::on_data(NetSocket* socket, const char* data, int32 length)
{
	log_info("CarSocketHandler::on_data get data: [%s]\n", data);

    http::HttpRequest request;
    if (!request.decode(data, length))
    {
    	log_warn("CarSocketHandler::on_data decode request error");
		return false;
    }

	if (request.get_content() == "")
	{
		log_error("pic is null");
		send_response(401, "pic is null", "");
		return true;
	}

	log_debug("CarSocketHandler::on_data method: %s, uri: %s, path: %s, query: %s\n",
			request.get_method().c_str(), request.get_uri().c_str(), request.get_path().c_str(),
			request.get_query().c_str());

	if ("/yihao01-car-api/parkApp/uploadCarPlateImage" == request.get_path())
	{
		std::string pic_name;
		if (!request.get_query("name", pic_name))
		{
			pic_name = "";
		}
		log_debug("get name: [%s] from request", pic_name.c_str());

		CarHttpMsg* msg = new CarHttpMsg();
		msg->msg_sn = sn_;
		msg->name = pic_name;
		msg->content = request.get_content();
		msg->msg_type = MSG_TYPE_CAT_HTTP_PLATE_RECOGNITION;
		msg->receive_time = get_cur_microsecond();
		thread_->put_data(msg);
	}
	else
	{
		log_error("url [%s] error", request.get_path().c_str());
		send_response(400, "url error", "");
		return true;
	}

	return true;
}

void CarSocketHandler::send_response(int code, const std::string& desc, const std::string& plate_num)
{
	Json::Value root;
	Json::FastWriter writer;
	root["code"] = code;
	root["desc"] = desc;
	root["data"]["platenum"] = plate_num;
	std::string response_content = writer.write(root);
	
	http::HttpResponse response;
	std::string content;
	
    response.set_status(200);
    response.set_content(response_content);
    response.encode(content);

    net_socket_->get_buffer_out()->write(content.c_str(), content.size());
	net_socket_->get_buffer_out()->flush();
}
