#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include "http_protocol.h"
#include <string>

namespace http
{
    class HttpResponse
    {
    public:
        HttpResponse();
        virtual ~HttpResponse();

    public:
        bool decode(const std::string& data);
        bool encode(std::string& data);

    public:
        const std::string& get_content() const;
        void set_content(const std::string& content);

        const std::string& get_version() const;
        void set_version(const std::string& version);

        int get_status() const;
        void set_status(int status);

        const std::string& get_reason() const;

        bool get_header(const std::string& key, std::string& value) const;
        bool get_header(const std::string& key, int& value) const;
        void set_header(const std::string& key, const std::string& value);
        void set_header(const std::string& key, int value);
        void remove_header(const std::string& key);

        const Header& get_headers() const;

    private:
        const char* read_status_line(const char* begin, const char* end);  // 读取http响应的第一行
        const char* read_version(const char* begin, const char* end);
        const char* read_space(const char* begin, const char* end);
        const char* read_status(const char* begin, const char* end);
        const char* read_reason(const char* begin, const char* end);
        const char* read_headers(const char* begin, const char* end);  // 读取头部
        const char* read_header(const char* begin, const char* end);  // 读取一个头部

    private:
        static const std::string& format_header_key(std::string& dst, const std::string& key);  // http头键值转换为 ”Xxxxxx-Xxxx-Xxxx“ 格式

    private:
        Header headers_;
        std::string version_;
        int status_;
        std::string reason_;
        std::string content_; 
    };
}

#endif  // __HTTP_RESPONSE_H__