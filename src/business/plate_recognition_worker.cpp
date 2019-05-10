#include "plate_recognition_worker.h"
#include "thread_macro.h"
#include "user_thread_macro.h"
#include "log.h"
#include "car_http_thread.h"
#include "general.h"

#include "car_algo/pd_api.h"


PlateRecognitionWorker* PlateRecognitionWorker::inst_ = new PlateRecognitionWorker();

PlateRecognitionWorker::PlateRecognitionWorker()
{

}

PlateRecognitionWorker::~PlateRecognitionWorker()
{

}

PlateRecognitionWorker* PlateRecognitionWorker::get_instance()
{
	return inst_;
}

bool PlateRecognitionWorker::init(int gpu_index)
{
	log_info("PD_Init on gpu [%d]", gpu_index);
	int ret = PD_Init(gpu_index);
	if (0 != ret)
	{
		log_error("PD_Init failed, ret [%d]", ret);
		return false;
	}

	return true;
}

void PlateRecognitionWorker::put_data(BaseMsg* msg)
{
	que_.put_data(msg);
}

void PlateRecognitionWorker::run()
{
	BaseMsg* msg = NULL;
    while (true)
    {
        msg = que_.get_data();

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
}

void PlateRecognitionWorker::dispose(BaseMsg* msg)
{
    if (NULL == msg)
    {
        return;
    }

    switch (msg->msg_type)
    {
        case MSG_TYPE_PLATE_RECOGNITION_REQUEST:
        {
            recognize_plate(msg);
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
            log_error("msg_type[%d] is invalied.", msg->msg_type);
            break;
        }
    }

    return;
}

void PlateRecognitionWorker::recognize_plate(BaseMsg* msg)
{
    PlateRecognitionRequestMsg* request_msg = (PlateRecognitionRequestMsg*)msg;

	log_debug("get pic heigth: [%d], width: [%d], name: [%s]",
		request_msg->height, request_msg->width, request_msg->name.c_str());

	std::string desc = "SUCCESS";
	std::string plate_num;
	
	int64 start_detect_time = get_cur_microsecond();
	int ret = -1;
	
	try
	{
		ret = PD_Detect(request_msg->height, request_msg->width, request_msg->img_data, plate_num, request_msg->name);
		if (0 != ret)
		{
			log_error("PD_Detect failed, ret [%d]", ret);
			plate_num = "";
			desc = "PD_Detect failed";
		}
	}
	catch(...)
	{
		log_error("PD_Detect cause exception");
		plate_num = "";
		desc = "PD_Detect failed";
	}
	
	log_info("PD_Detect cost [%d]ms, ret [%d], plate_num [%s]", 
		get_cur_microsecond() - start_detect_time, ret, plate_num.c_str());

	PlateRecognitionResponseMsg* response_msg = new PlateRecognitionResponseMsg();
	response_msg->msg_type = MSG_TYPE_PLATE_RECOGNITION_RESPONSE;
	response_msg->msg_sn = request_msg->msg_sn;
	response_msg->receive_time = request_msg->receive_time;
	response_msg->code = ret;
	response_msg->desc = desc;
	response_msg->plate_num = plate_num;
	CarHttpThread::get_instance()->put_data(response_msg);
}

