/*
 * net_address.h
 * IP地址类
 */


#ifndef __NET_ADDRESS_H__
#define __NET_ADDRESS_H__

#include "platform.h"

#include <string>

class NetAddress
{
public:
    NetAddress();
    NetAddress(uint16 port);
    NetAddress(const std::string& ipaddress, uint16 port);
    NetAddress(uint32 ipaddress, uint16 port);
    NetAddress(const NetAddress& other);
    ~NetAddress();

public:
    NetAddress& operator=(const NetAddress& other);

public:
    // 获取网络字节序IP地址
    uint32 get_ipaddress() const;

    // 由网络字节序IP地址进行设置
    void set_ipaddress(uint32 ipaddress);

    // 获取主机字节序端口号
    uint16 get_port() const;

    // 由主机字节序端口号进行设置
    void set_port(uint16 port);

    // 获取点分十进制IP地址
    std::string get_format_ipaddress() const;

    // 由点分十进制IP地址进行设置
    void set_ipaddress(const std::string& ipaddress);

    // 由网络字节序IP地址得到点分十进制IP地址
    static std::string to_format_ipaddress(uint32 ipaddress);

    // 由点分十进制IP地址得到网络字节序IP地址
    static uint32 to_net_ipaddress(const std::string& ipaddress);

private:
    uint32 ipaddress_;  // 网络字节序IP地址
    uint16 port_;  // 主机字节序端口号
};

#endif  // __NET_ADDRESS_H__