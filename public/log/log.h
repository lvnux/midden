/*
 * log.h
 * 日志打印，按日期拆分日志文件
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <mutex>


#define MAX_LOG_LENGTH 4096

const std::string LEVEL_NAME[] =
{
    "DEBUG",
    "INFO",
    "NOTICE",
    "WARN",
    "ERROR",
    "CRIT",
    "ALERT",
    "EMERG"
};


enum Level
{
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_NOTICE,
    LEVEL_WARN,
    LEVEL_ERROR,
    LEVEL_CRIT,
    LEVEL_ALERT,
    LEVEL_EMERG
};


class CLog
{
private:
    CLog();
    ~CLog();

public:
    static CLog* get_instance();
    bool init(const std::string& path, const std::string& name, int level);
    bool open();
    void write_log(Level level, const char* file, const int line, const char* func, const char* format, ...);
    int get_level();
    void set_level(Level level);

private:
    void new_day_log_file();
    
private:
    std::string path_;
    std::string name_;
    Level level_;
    FILE* handler_;
    struct tm current_day_;
    pthread_mutex_t mutex_;

private:
    static CLog* inst_;
};

#define log_debug(...) CLog::get_instance()->write_log(LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_info(...) CLog::get_instance()->write_log(LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_notice(...) CLog::get_instance()->write_log(LEVEL_NOTICE, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_warn(...) CLog::get_instance()->write_log(LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_error(...) CLog::get_instance()->write_log(LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_crit(...) CLog::get_instance()->write_log(LEVEL_CRIT, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_alert(...) CLog::get_instance()->write_log(LEVEL_ALERT, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_emerg(...) CLog::get_instance()->write_log(LEVEL_EMERG, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#endif  // __LOG_H__