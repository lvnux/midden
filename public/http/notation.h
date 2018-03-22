/*
 * notation.h
 * 解析HTTP常用标记
 */

#ifndef __NOTATION_H__
#define __NOTATION_H__

namespace http
{
    class Notation
    {
    public:
        static bool is_upper(char ch);
        static bool is_lower(char ch);
        static bool is_alpha(char ch);
        static bool is_digit(char ch);
        static bool is_control(char ch);  // 控制符  '0' - '31'
        static bool is_carriage_return(char ch);
        static bool is_line_feed(char ch);
        static bool is_space(char ch);
        static bool is_tab(char ch);
        static bool is_dquote(char ch);
        static bool is_separator(char ch);
        static bool is_hex(char ch);

        static const char* check_CRLF(const char* begin, const char* end);  // 校验回车换行符
        static const char* check_blank(const char* begin, const char* end);  // 校验空白字符（回车换行、空行或TAB）
        static const char* check_text(const char* begin, const char* end);  // 校验非控制符
        static const char* check_token(const char* begin, const char* end);  // 校验分隔符
    };
}

#endif  // __NOTATION_H__