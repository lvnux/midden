#include "http_response.h"
#include "general.h"
#include "notation.h"


namespace http
{
    HttpResponse::HttpResponse()
     : version_("HTTP/1.1"), status_(0)
    {

    }

    HttpResponse::~HttpResponse()
    {

    }

    /*
     * 解码，按照http协议对响应进行解码
     */
    bool HttpResponse::decode(const std::string& data)
    {
        const char* begin = data.c_str();
        const char* end = begin + data.size();

        begin = read_status_line(begin, end);
        begin = read_headers(begin, end);
        
        if (NULL == begin)
        {
            printf("http response decode data [%s] error\n", data.c_str());
            return false;
        }

        int content_length = end - begin;
        int value;
        if (get_header("Content-Length", value) && (value <= end - begin))
        {
            content_length = value;
        }

        content_.assign(begin, content_length);
        return true;
    }

    /*
     * 编码，按照http协议对编码响应内容
     */
    bool HttpResponse::encode(std::string& data)
    {
        set_header("Content-Type", "text/html; charset=UTF-8");
        data.reserve(2048);
        data.append(version_).append(" ").append(to_string(status_)).append(" ").append(reason_).append(" ").append("\r\n");  // eg: HTTP/1.1 200 OK
        for (Header::const_iterator itor = headers_.begin(); itor != headers_.end(); ++itor)
        {
            data.append(itor->first).append(":").append(itor->second).append("\r\n");  // eg: Content-Type: text/html; charset=UTF-8
        }
        data.append("\r\n");
        data.append(content_);

        return true;
    }

    const std::string& HttpResponse::get_content() const
    {
        return content_;
    }

    void HttpResponse::set_content(const std::string& content)
    {
        content_ = content;
        set_header("Content-Length", content_.size());
    }

    const std::string& HttpResponse::get_version() const
    {
        return version_;
    }

    void HttpResponse::set_version(const std::string& version)
    {
        version_ = version;
    }

    int HttpResponse::get_status() const
    {
        return status_;
    }

    void HttpResponse::set_status(int status)
    {
        status_ = status;
    }

    const std::string& HttpResponse::get_reason() const
    {
        return reason_;
    }

    bool HttpResponse::get_header(const std::string& key, std::string& value) const
    {
        std::string format_key;
        Header::const_iterator itor = headers_.find(format_header_key(format_key, key));
        if (itor != headers_.end())
        {
            value = itor->second;
            return true;
        }

        return false;
    }

    bool HttpResponse::get_header(const std::string& key, int& value) const
    {
        std::string temp;
        if (get_header(key, temp))
        {
            value = atoi(temp.c_str());
            return true;
        }

        return false;
    }

    void HttpResponse::set_header(const std::string& key, const std::string& value)
    {
        std::string format_key;
        headers_[format_header_key(format_key, key)] = value;
    }

    void HttpResponse::set_header(const std::string& key, int value)
    {
        set_header(key, to_string(value));
    }

    void HttpResponse::remove_header(const std::string& key)
    {
        std::string format_key;
        Header::iterator itor = headers_.find(format_header_key(format_key, key));
        if (itor != headers_.end())
        {
            headers_.erase(itor);
        }
    }

    const Header& HttpResponse::get_headers() const
    {
        return headers_;
    }

    const char* HttpResponse::read_status_line(const char* begin, const char* end)
    {
        begin = read_version(begin, end);
        begin = read_space(begin, end);
        begin = read_status(begin, end);
        begin = read_space(begin, end);
        begin = read_reason(begin, end);

        return begin;
    }

    const char* HttpResponse::read_version(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        for (const char* pos = begin; pos < end; ++pos)
        {
            if (Notation::is_space(pos[0]))
            {
                if (pos != begin)
                {
                    version_.assign(begin, pos);
                    return pos;
                }
                break;
            }
        }

        return NULL;
    }

    const char* HttpResponse::read_space(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        if (Notation::is_space(begin[0]))
        {
            return begin + 1;
        }

        return NULL;
    }

    const char* HttpResponse::read_status(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        if (!Notation::is_digit(begin[0]))
        {
            return NULL;
        }

        status_ = begin[0] - '0';

        while (++begin < end)
        {
            if (!Notation::is_digit(begin[0]))
            {
                return begin;
            }
            status_ = 10 * status_ + begin[0];
        }

        return NULL;
    }

    const char* HttpResponse::read_reason(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        for (const char* pos = begin; pos + 1 < end; ++pos)
        {
            if (Notation::is_carriage_return(pos[0]) && Notation::is_line_feed(pos[0]))
            {
                reason_.assign(begin, pos - begin);
                return pos;
            }
        }

        return NULL;
    }

    const char* HttpResponse::read_headers(const char* begin, const char* end)
    {
        const char* pos;
        while ((begin < end) && (NULL != begin))
        {
            pos = Notation::check_CRLF(begin, end);  // 头部每一行后面的回车换行符
            if (NULL == pos)
            {
                return NULL;
            }
            begin = pos;
            pos = Notation::check_CRLF(begin, end);  // 判断该行是否是空行，不是空行就读取该行里的头部信息，是空行表示头部读取完成
            if (NULL == pos)  // 不是空行
            {
                begin = read_header(begin, end);
            }
            else
            {
                begin = pos;
                break;
            }
        }

        return begin;
    }

    const char* HttpResponse::read_header(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        std::string key;  // 头部属性名称
        const char* key_pos_end = Notation::check_token(begin, end);  // 头部属性名称最后一个字符的位置的后一位
        if (NULL == key_pos_end)
        {
            return NULL;
        }
        else
        {
            key.assign(begin, key_pos_end - begin);
        }

        begin = key_pos_end;
        if ((begin < end) && (NULL != begin) && (':' == begin[0]))
        {
            ++begin;  // 跳过":"
        }
        else
        {
            return NULL;
        }

        std::string value;
        bool readed;
        const char* pos;
        while ((begin < end) && (NULL != begin))
        {
            pos = begin;
            readed = false;

            // 检查空白字符，为了共用readed标识符，将检查空白字符的放在前面
            while (NULL != pos)
            {
                pos = Notation::check_blank(begin, end);
                if (NULL != pos)
                {
                    readed = true;
                    begin = pos;
                }
            }
            if (readed && !value.empty())
            {
                value.append(" ");
            }

            // 检查头部属性值
            pos = Notation::check_text(begin, end);
            if (NULL != pos)
            {
                readed = true;
                value.append(begin, pos - begin);
                begin = pos;
            }
            
            if (!readed)
            {
                break;
            }
        }

        set_header(key, value);
        return begin;
    }

    /**
     * http头键值转换为 ”Xxxxxx-Xxxx-Xxxx“ 格式
     */
    const std::string& HttpResponse::format_header_key(std::string& dst, const std::string& key)
    {
        bool need_to_upper = true;
        dst = key;
        for (std::string::iterator itor = dst.begin(); itor != dst.end(); ++itor)
        {
            if (need_to_upper)
            {
                *itor = toupper(*itor);
                need_to_upper = false;
            }
            else if ('-' != *itor)
            {
                *itor = tolower(*itor);
            }

            if ('-' == *itor)
            {
                need_to_upper = true;
            }
        }

        return dst;
    }
}