#include "main.h"
#include "log.h"
#include "inifile.h"
#include "car_http_thread.h"
#include "plate_recognition_worker.h"

#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    std::string conf = "../conf/car_node.conf";
	int oc;
	while ((oc = getopt(argc, argv, "c:")) != -1)
	{
		switch (oc)
		{
			case 'c':
				conf.assign(optarg);
				break;
			default:
				printf("Usage: %s [-c conf]\n", argv[0]);
                return -1;
		}
	}

    // 配置文件解析
	if (false == parse_conf(conf.c_str()))
	{
		printf("parse_conf failed\n");
		return -1;
	}

    // 初始化日志记录对象
	if (false == init_log())
	{
		printf("init_log failed\n");
		return -1;
	}

	if (false == start_car_node_thread())
	{
		printf("start_car_node_thread failed\n");
		return -1;
	}

	if (false == PlateRecognitionWorker::get_instance()->init(IniFile::get_instance()->get_int("algo", "gpu_index")))
	{
		printf("PlateRecognitionWorker::get_instance()->init failed\n");
		return -1;
	}
	
	PlateRecognitionWorker::get_instance()->run();

    return 0;
}

bool parse_conf(const char* file)
{
	return IniFile::get_instance()->open(file);
}

bool init_log()
{
	std::string log_path = IniFile::get_instance()->get_string("log", "path");
	std::string log_filename = IniFile::get_instance()->get_string("log", "filename");
	int log_level = IniFile::get_instance()->get_int("log", "level");
	if (log_level < (int)LEVEL_DEBUG || log_level > (int)LEVEL_EMERG)
	{
		printf("log level [%d] error\n", log_level);
		return false;
	}

	return CLog::get_instance()->init(log_path, log_filename, log_level);
}

bool start_car_node_thread()
{
	std::string ipaddress = IniFile::get_instance()->get_string("server", "ipaddress");
	int port = IniFile::get_instance()->get_int("server", "port");

	
    CarHttpThread::get_instance()->init(ipaddress, port);
    CarHttpThread::get_instance()->start();

	return true;
}

