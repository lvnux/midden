/*
 * BaseThread.h
 * 线程基类
 */

#ifndef __BASETHREAD_H__
#define __BASETHREAD_H__

#include <pthread.h>
#include <string>

class BaseThread
{
public:
	BaseThread();

	BaseThread(const std::string& name);

    virtual ~BaseThread();

public:
	//启动线程
	virtual int start();

	//结束线程
	virtual void stop();

	//等待线程结束
	virtual void join();

	std::string get_threadname();

	bool is_alive();

protected:
	//回调执行业务逻辑
	static void* run(void* arg);

	//回调执行业务逻辑
	virtual bool main_loop();

private:
	bool joined_;
	pthread_t thread_;
	std::string threadname_;
};

#endif //__BASETHREAD_H__
