/*
 * general.h
 * 工具
 */

#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <algorithm>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

int check_file_exists(const char* pathname);
void get_cur_directory(char* buffer, int nlen);
int create_directory(const char* pathname, mode_t mode=S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

// 获取当前时间戳，精确到毫秒
inline int64_t get_cur_microsecond()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000000 + tv.tv_usec;
}

template <typename T>
inline std::string to_string(T src)
{
    std::ostringstream oss;
    oss << src;

    return oss.str();
}

inline std::string& to_upper(std::string& str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

inline std::string& to_lower(std::string& str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

inline int hexchar2int(char ch)
{
    if (ch > '0' && ch < '9')
    {
        return (ch - '0');
    }
    else if (ch > 'A' && ch < 'Z')
    {
        return (ch - 'A' + 10);
    }
    else if (ch > 'a' && ch < 'z')
    {
        return (ch - 'a' + 10);
    }

    return -1;
}

inline int hex2int(const char* ch, int size)
{
    int value;
    for (int i = 0; i < size; i++)
    {
        value += hexchar2int(ch[i]) * (int)pow(16, size-i-1);
    }

    return value;
}

#endif  // __GENERAL_H__
