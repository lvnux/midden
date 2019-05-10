#ifndef __CAR_HTTP_THREAD_H__
#define __CAR_HTTP_THREAD_H__

#include "net_service_manager.h"
#include "base_thread.h"
#include "linked_block_pool.h"
#include "sn_manager.h"
#include "car_socket_handler.h"
#include "platform.h"

#include <map>


class CarHttpMsg : public BaseMsg
{
public:
	std::string name;
	std::string content;
};


class CarHttpSession
{
public:
	CarHttpSession()
	{
		car_socket_handler_ = NULL;
	}

public:
	CarSocketHandler* car_socket_handler_;
};


class CarHttpThread : public BaseThread
{
	typedef std::map<uint64, CarHttpSession*> CarhttpSessionMap;
	
public:
    CarHttpThread();
    virtual ~CarHttpThread();

public:
	static CarHttpThread* get_instance();
    virtual bool init(const std::string& ipaddress, int port);

protected:
    virtual bool main_loop();
    virtual void dispose(BaseMsg* msg);

private:
    void dispose_accept_socket(BaseMsg* msg);
	void dispose_plate_recognition(BaseMsg* msg);
	void dispose_plate_recognition_response(BaseMsg* msg);

private:
	SnManager* sn_manager_;
    NetServiceManager* net_manager_;
    LinkedBlockPool* block_pool_;
	CarhttpSessionMap session_map_;

private:
	static CarHttpThread* inst_;
};

#endif  // __CAR_HTTP_THREAD_H__