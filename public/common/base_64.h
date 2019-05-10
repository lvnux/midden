/*
 * base_64.h
 * 工具
 */


#ifndef __BASE_64_H__
#define __BASE_64_H__

#include <iostream>
#include <string>
#include <cstring>


class Base64
{
public:
    Base64();
	~Base64();
    
    /**
     * 这里必须是unsigned类型，否则编码中文的时候出错
     */
    std::string Encode(const unsigned char * str,int bytes);
    std::string Decode(const char *str,int bytes);
    void Debug(bool open = true);

private:
	std::string _base64_table;
	static const char base64_pad = '=';

};

#endif  // __BASE_64_H__

