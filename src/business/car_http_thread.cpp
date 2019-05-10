#include "car_http_thread.h"
#include "net_server_socket.h"
#include "thread_macro.h"
#include "user_thread_macro.h"
#include "log.h"
#include "plate_recognition_worker.h"
#include "opencv2/opencv.hpp"
#include "general.h"

#include <vector>


CarHttpThread* CarHttpThread::inst_ = new CarHttpThread();

CarHttpThread::CarHttpThread()
    : BaseThread(), sn_manager_(NULL), net_manager_(NULL), block_pool_(NULL)
{

}

CarHttpThread::~CarHttpThread()
{

}

CarHttpThread* CarHttpThread::get_instance()
{
	return inst_;
}

bool CarHttpThread::init(const std::string& ipaddress, int port)
{
    if (false == BaseThread::init())
    {
        printf("BaseThread::init() failed\n");
        return false;
    }

	sn_manager_ = new SnManager();
	if (NULL == sn_manager_)
	{
		printf("new SnManager() failed\n");
		return false;
	}

    net_manager_ = new NetServiceManager();
    if (false == net_manager_->init())
    {
        printf("manager_->init() failed\n");
        return false;
    }

	block_pool_ = new LinkedBlockPool();
	if (NULL == block_pool_)
	{
		printf("new LinkedBlockPool() failed\n");
        return false;
	}
	
    NetServerSocket* server = net_manager_->create_server_socket(this);
    if (false == server->listen(NetAddress(ipaddress, port)))
    {
        printf("server->listen() failed\n");
        return false;
    }

    return true;
}

bool CarHttpThread::main_loop()
{
    BaseMsg* msg = NULL;
    while (true)
    {
        timer_manager_->click();
        net_manager_->get_selector()->select();

        pthread_mutex_lock(&mutex_);
        msg = que_.get_data();
        pthread_mutex_unlock(&mutex_);

        if (NULL == msg)
        {
            usleep(1000 * 10);
        }
        else
        {
            dispose(msg);
            delete msg;
        }
    }

    return true;
}

void CarHttpThread::dispose(BaseMsg* msg)
{
    if (NULL == msg)
    {
        return;
    }

    switch (msg->msg_type)
    {
        case MSG_TYPE_ACCEPT_SOCKET:
        {
            dispose_accept_socket(msg);
            break;
        }
		case MSG_TYPE_CAT_HTTP_PLATE_RECOGNITION:
		{
			dispose_plate_recognition(msg);
			break;
		}
		case MSG_TYPE_PLATE_RECOGNITION_RESPONSE:
		{
			dispose_plate_recognition_response(msg);
			break;
		}
        case MSG_TYPE_HTTP_RESPONSE:
        {
            break;
        }
        case MSG_TYPE_TIMEOUT:
        {
            break;
        }
        default:
        {
            log_error("msg_type[%lu] is invalied.", msg->msg_type);
            break;
        }
    }

    return;
}

void CarHttpThread::dispose_accept_socket(BaseMsg* msg)
{
    TAcceptSocketMsg* accept_socket_msg = (TAcceptSocketMsg*)msg;
    // printf("msg type: [%d]\n", pMsg->msg_type);

	uint64 sn = sn_manager_->get_sn();
    CarSocketHandler* handler = new CarSocketHandler(this, sn);
    if (false == handler->init(net_manager_, accept_socket_msg->socket, accept_socket_msg->address, block_pool_))
    {
        log_error("TestThread::dispose_accept_socket failed");
        delete handler;
        return;
    }

	CarHttpSession* session = new CarHttpSession();
	session->car_socket_handler_ = handler;
	session_map_[sn] = session;
}

void CarHttpThread::dispose_plate_recognition(BaseMsg* msg)
{
	CarHttpMsg* car_msg = (CarHttpMsg*)msg;
	
	std::vector<unsigned char> img_data(car_msg->content.begin(), car_msg->content.end());
	cv::Mat img_decode = cv::imdecode(img_data, CV_LOAD_IMAGE_COLOR); 

	PlateRecognitionRequestMsg* request_msg = new PlateRecognitionRequestMsg();
	request_msg->msg_type = MSG_TYPE_PLATE_RECOGNITION_REQUEST;
	request_msg->msg_sn = car_msg->msg_sn;
	request_msg->receive_time = car_msg->receive_time;
	request_msg->height = img_decode.rows;
	request_msg->width = img_decode.cols;
	request_msg->img_data = img_decode.data;
	request_msg->name = car_msg->name;
	PlateRecognitionWorker::get_instance()->put_data(request_msg);
}

void CarHttpThread::dispose_plate_recognition_response(BaseMsg* msg)
{
	PlateRecognitionResponseMsg* response_msg = (PlateRecognitionResponseMsg*)msg;
	CarhttpSessionMap::iterator itor = session_map_.find(response_msg->msg_sn);
	if (itor == session_map_.end())
	{
		log_warn("can not find session fro msg_sn [%lu]", response_msg->msg_sn);
		return;
	}

	itor->second->car_socket_handler_->send_response(response_msg->code, response_msg->desc, response_msg->plate_num);

	log_info("total cost [%d]ms", get_cur_microsecond() - response_msg->receive_time);

	if (NULL != itor->second->car_socket_handler_)
	{
		delete itor->second->car_socket_handler_;
		itor->second->car_socket_handler_ = NULL;
	}

	delete itor->second;
	session_map_.erase(itor);
}