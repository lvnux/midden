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
    uint32 get_ipaddress() const;
    void set_ipaddress(uint32 ipaddress);

    uint16 get_port() const;
    void set_port(uint16 port);

    std::string get_format_ipaddress() const;
    void set_ipaddress_form_format_ipaddress(const std::string& ipaddress);

    static std::string to_format_ipaddress(uint32 ipaddress);
    static uint32 to_net_ipaddress(const std::string& ipaddress);

private:
    uint32 ipaddress_;
    uint16 port_;
};

#endif  // __NET_ADDRESS_H__