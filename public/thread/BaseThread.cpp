#include "BaseThread.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <errno.h>


BaseThread::BaseThread()
	: threadname_("thread")
{

}

BaseThread::BaseThread(const std::string& name)
	: threadname_(name)
{

}

BaseThread::~BaseThread()
{

}

int BaseThread::start()
{
	pthread_attr_t attr;
	memset(&attr, 0, sizeof(pthread_attr_t));
	if (0 != pthread_attr_init(&attr))
	{
		std::cout << "pthread_attr_init failed" << std::endl;
		return -1;
	}

	if (joined_ == false)
	{
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	}

	if (pthread_create(&thread_, &attr, run, this) != 0)
	{
		std::cout << "create thread failed" <<  std::endl;
		return -1;
	}

	int rc = pthread_setname_np(thread_, get_threadname().c_str());
	if (0 != rc)
	{
		std::cout << "set thread name " << get_threadname() << " failed" << std::endl;
	}
	char threadname[16] = { 0 };
	pthread_getname_np(thread_, threadname, 16);
	std::cout << "set thread name " << get_threadname() << " successfully" << std::endl;

	if (0 != pthread_attr_destroy(&attr))
	{
		std::cout << "set attr destroy failed" << std::endl;
	}
	
	return 0;
}

void BaseThread::stop()
{
	pthread_cancel(thread_);
}

void BaseThread::join()
{
	pthread_join(thread_, NULL);
}

bool BaseThread::is_alive()
{
	int rc = pthread_kill(thread_, 0);
	if (rc == ESRCH || rc == EINVAL)
	{
		return false;
	}

	return true;
}

std::string BaseThread::get_threadname()
{
	return threadname_;
}

void* BaseThread::run(void* arg)
{
	BaseThread* ptr = (BaseThread*)arg;
	while (ptr->is_alive())
	{
		if (false == ptr->main_loop())
			break;
	}
	
	return NULL;
}

bool BaseThread::main_loop()
{
	return false;
}
