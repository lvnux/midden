#include "net_address.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


NetAddress::NetAddress()
{

}

NetAddress::NetAddress(uint16 port)
    : port_(port)
{
    struct sockaddr_in addr;
    inet_aton("127.0.0.1", &addr.sin_addr);
    ipaddress_ = addr.sin_addr.s_addr;
}

NetAddress::NetAddress(const std::string& ipaddress, uint16 port)
    : port_(port)
{
    hostent* host = gethostbyname(ipaddress.c_str());
    if (NULL == host)
    {
        ipaddress_ = 0;
    }
    else
    {
        ipaddress_ = *reinterpret_cast<uint32*>(host->h_addr);
    }
}

NetAddress::NetAddress(uint32 ipaddress, uint16 port)
    : ipaddress_(ipaddress), port_(port)
{

}

NetAddress::NetAddress(const NetAddress& other)
    : ipaddress_(other.ipaddress_), port_(other.port_)
{

}

NetAddress::~NetAddress()
{

}

NetAddress& NetAddress::operator=(const NetAddress& other)
{
    ipaddress_ = other.ipaddress_;
    port_ = other.port_;

    return *this;
}

uint32 NetAddress::get_ipaddress() const
{
    return ipaddress_;
}

void NetAddress::set_ipaddress(uint32 ipaddress)
{
    ipaddress_ = ipaddress;
}

uint16 NetAddress::get_port() const
{
    return port_;
}

void NetAddress::set_port(uint16 port)
{
    port_ = port;
}

std::string NetAddress::get_format_ipaddress() const
{
    in_addr addr;
    addr.s_addr = ipaddress_;
    return inet_ntoa(addr);
}

void NetAddress::set_ipaddress_form_format_ipaddress(const std::string& ipaddress)
{
    hostent* host = gethostbyname(ipaddress.c_str());
    if (NULL == host)
    {
        ipaddress_ = 0;
    }
    else
    {
        ipaddress_ = *reinterpret_cast<uint32*>(host->h_addr);
    }
}

std::string NetAddress::to_format_ipaddress(uint32 ipaddress)
{
    in_addr addr;
    addr.s_addr = ipaddress;
    return inet_ntoa(addr);
}

uint32 NetAddress::to_net_ipaddress(const std::string& ipaddress)
{
    hostent* host = gethostbyname(ipaddress.c_str());
    if (NULL == host)
    {
        return 0;
    }
    else
    {
        return *reinterpret_cast<uint32*>(host->h_addr);
    }
}
