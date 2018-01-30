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
#include <vector>


int daemon_init();
int check_file_exists(const char* pszfilename);
void get_cur_directory(char* szpath, int nlen);

std::string trim(const std::string& src);
void split_to_vec(std::string src, std::string separator, std::vector<std::string>& vec);

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

#endif  // __GENERAL_H__
