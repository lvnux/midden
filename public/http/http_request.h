/*
 * http_request.h
 * 解析、构造http请求
 */

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "http_protocol.h"
#include "platform.h"
#include <string>

namespace http
{
    class HttpRequest
    {
    public:
        HttpRequest();
        virtual ~HttpRequest();

    public:
        bool decode(const char* data, int32 length);
        bool encode(std::string& data);

    public:
        const std::string& get_method() const;
        void set_method(const std::string& path);

        const std::string& get_uri() const;
        void set_uri(const std::string& uri);

        const std::string& get_path() const;
		const std::string& get_query() const;
		bool get_query(const std::string& key, std::string& value) const;
		bool get_query(const std::string& key, int& value) const;

        const std::string& get_version() const;
        void set_version(const std::string& version);

        bool get_header(const std::string& key, std::string& value) const;
        bool get_header(const std::string& key, int& value) const;
        void set_header(const std::string& key, const std::string& value);
        void set_header(const std::string& key, int value);
        void remove_header(const std::string& key);
        const Header& get_headers() const;

        const std::string& get_content() const;
        void set_content(const std::string& content);

        const std::string& get_host() const;

    private:
        const char* read_request_line(const char* begin, const char* end);  // 读取http请求的第一行
        const char* read_method(const char* begin, const char* end);
        const char* read_space(const char* begin, const char* end);
        const char* read_uri(const char* begin, const char* end);
        const char* read_version(const char* begin, const char* end);
        const char* read_headers(const char* begin, const char* end);  // 读取头部
        const char* read_header(const char* begin, const char* end);  // 读取一个头部
        void parse_querys(std::string querys);

    private:
        static const std::string& format_header_key(std::string& dst, const std::string& key);  // http头键值转换为 ”Xxxxxx-Xxxx-Xxxx“ 格式

    private:
        std::string uri_;
        std::string method_;
        std::string path_;
		std::string query_;
        std::string version_;
        Header headers_;
		Query querys_;
        std::string content_;
        std::string host_;
    };
}

#endif  // __HTTP_REQUEST_H__