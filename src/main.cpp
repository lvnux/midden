#include "main.h"
#include "log.h"
#include "test_thread.h"
#include "inifile.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    std::string conf = "../conf/midden.conf";
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

	start_test();

    while (true)
    {
        sleep(2);
    }

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

bool start_test()
{
	std::string ipaddress = IniFile::get_instance()->get_string("server", "ipaddress");
	int port = IniFile::get_instance()->get_int("server", "port");

	TestThread* test = new TestThread();
    test->init(ipaddress, port);
    test->start();

	return true;
}