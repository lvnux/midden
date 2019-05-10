/*
 * http_protcol.h
 * 内部协议
 */

#ifndef __HTTP_PROTOCOL_H__
#define __HTTP_PROTOCOL_H__

#include <map>
#include <string>

namespace http
{
    typedef std::map<std::string, std::string> Header;
	typedef std::map<std::string, std::string> Query;
}

#endif  // __HTTP_PROTOCOL_H__