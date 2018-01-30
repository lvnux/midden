#include "log.h"

#include <string.h>
#include <stdarg.h>

#define LOG_LEN 4096
#define FLIE_INFO_LEN 256

CLog* CLog::inst_ = new CLog();

CLog::CLog()
{
    pthread_mutex_init(&mutex_, NULL);
}

CLog::~CLog()
{
    pthread_mutex_destroy(&mutex_);
}

CLog* CLog::get_instance()
{
    return inst_;
}

bool CLog::init(const std::string& path, const std::string& name, int level)
{
    path_ = path;
    name_ = name;
    level_ = (Level)level;
    return open();
}

bool CLog::open()
{
    if (NULL == handler_)
    {
        time_t timestamp = time(NULL);
        localtime_r(&timestamp, &current_day_);

        char format_time[60] = { 0 };
        snprintf(format_time, 60, "%s_%04d%02d%02d", name_.c_str(), 
            (1900 + current_day_.tm_year), (1 + current_day_.tm_mon), 
            current_day_.tm_mday);

        std::string full_path;
        std::string link_name;
        full_path.assign(path_).append("/").append(format_time).append(".log");
        link_name.assign(path_).append("/").append(name_).append(".log");
        unlink(link_name.c_str());
        symlink(full_path.c_str(), link_name.c_str());

        if ( NULL == (handler_ = fopen(full_path.c_str(), "a")))
        {
            return false;
        }

        setvbuf(handler_, NULL, _IONBF, 0);
    }

    return true;
}


void CLog::write_log(Level level, const char* file, const int line, const char* func, const char* format, ...)
{
    if (level < level_)
        return;

    struct tm now = { 0 };
    time_t timestamp = time(NULL);
    localtime_r(&timestamp, &now);
    if (now.tm_year > current_day_.tm_year ||
        now.tm_mon  > current_day_.tm_mon  ||
        now.tm_mday > current_day_.tm_mday)
    {
        new_day_log_file();
    }

    va_list va;
    char msg[MAX_LOG_LENGTH] = { 0 };
    va_start(va, format);
    vsnprintf(msg, sizeof(msg), format, va);
    va_end(va);

    char filename[128] = { 0 };
    char* p = strrchr(const_cast<char*>(file), '/');
    if (NULL == p)
    {
        snprintf(filename, 128, "%s", file);
    }
    else
    {
        snprintf(filename, 128, "%s", p+1);
    }
    char soruce_position[256] = { 0 };
    snprintf(soruce_position, 256, "%s:%d:%s", filename, line, func);
    
    if (NULL != handler_)
    {
        fprintf(handler_, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] [%s] %s\n", 
            (1900 + current_day_.tm_year), (1 + current_day_.tm_mon), 
            current_day_.tm_mday, current_day_.tm_hour,
            current_day_.tm_min, current_day_.tm_sec,
            LEVEL_NAME[level_].c_str(), 
            soruce_position, msg);
        fflush(handler_);
    }
}

void CLog::new_day_log_file()
{
    if (handler_)
    {
        fclose(handler_);
        handler_ = NULL;
    }

    open();
}

Level CLog::get_level()
{
    return level_;
}

void CLog::set_level(Level level)
{
    if (level <= LEVEL_EMERG)
    {
        pthread_mutex_lock(&mutex_);
        level_ = level;
        pthread_mutex_unlock(&mutex_);
    }
}