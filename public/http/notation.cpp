#include "notation.h"
#include <stdlib.h>

namespace http
{
    bool Notation::is_upper(char ch)
    {
        return (ch >= 'A') && (ch <= 'Z');
    }

    bool Notation::is_lower(char ch)
    {
        return (ch >= 'a') && (ch <= 'z');
    }

    bool Notation::is_alpha(char ch)
    {
        return is_upper(ch) || is_lower(ch);
    }

    bool Notation::is_digit(char ch)
    {
        return (ch >= '0') && (ch <= '9');
    }

    bool Notation::is_control(char ch)
    {
        return (ch >= 0) && (ch <= 31);
    }

    bool Notation::is_carriage_return(char ch)
    {
        return '\r' == ch;
    }

    bool Notation::is_line_feed(char ch)
    {
        return '\n' == ch;
    }

    bool Notation::is_space(char ch)
    {
        return ' ' == ch;
    }

    bool Notation::is_tab(char ch)
    {
        return '\t' == ch;
    }

    bool Notation::is_dquote(char ch)
    {
        return '"' == ch;
    }

    bool Notation::is_separator(char ch)
    {
        char separators[] = {
            '<',    '>',    '(',    ')',
            '[',    ']',    '{',    '}',
            '@',    ',',    ';',    ':',
            '\\',   '"',    '/',    '?',
            '=',    ' ',    '\t',
            '\0'  // 最后一个用\0，便于遍历
        };

        for (char* p = separators; *p != '\0'; p++)
        {
            if (ch == *p)
            {
                return true;
            }
        }

        return false;
    }

    bool Notation::is_hex(char ch)
    {
        if (((ch >= '0') && (ch <= '9')) ||
            ((ch >= 'a') && (ch <= 'f')) ||
            ((ch >= 'A') && (ch <= 'F')))
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    const char* Notation::check_CRLF(const char* begin, const char* end)
    {
        if ((begin+1 >= end) || (NULL == begin))
        {
            return NULL;
        }

        if (is_carriage_return(begin[0]) && is_line_feed(begin[1]))
        {
            return begin + 2;
        }

        return NULL;
    }

    // 检查http头属性值里是否有间隔的空格，类似 Date: Wed, 21 Mar 2018 02:33:54 GMT
    const char* Notation::check_blank(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        const char* pos = check_CRLF(begin, end);
        if (NULL == pos)
        {
            pos = begin;
        }

        if (!is_tab(pos[0]) && !is_space(pos[0]))
        {
            return NULL;
        }

        while (++pos < end)
        {
            if (!is_tab(pos[0]) && !is_space(pos[0]))
            {
                return pos;
            }
        }

        return NULL;
    }

    const char* Notation::check_text(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        if (is_control(begin[0]))  // 无有效字符
        {
            return NULL;
        }

        while (++begin < end)
        {
            if (is_control(begin[0]))
            {
                return begin;
            }
        }

        return NULL;
    }

    const char* Notation::check_token(const char* begin, const char* end)
    {
        if ((begin >= end) || (NULL == begin))
        {
            return NULL;
        }

        if (is_control(begin[0]) || is_separator(begin[0]))
        {
            return NULL;
        }

        while (++begin < end)
        {
            if (is_control(begin[0]) || is_separator(begin[0]))
            {
                return begin;
            }
        }

        return NULL;
    }
}