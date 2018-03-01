/*
 * endianEx.h
 * 大小端格式转换
 */

#ifndef __EDNIAN_EX_H__
#define __EDNIAN_EX_H__

#include "platform.h"

namespace pack
{
    class IEndian
    {
    public:
        virtual uint64 from_big(uint64 value) = 0;
        virtual uint32 from_big(uint32 value) = 0;
        virtual uint16 from_big(uint16 value) = 0;

        virtual uint64 from_little(uint64 value) = 0;
        virtual uint32 from_little(uint32 value) = 0;
        virtual uint16 from_little(uint16 value) = 0;

        virtual uint64 to_big(uint64 value) = 0;
        virtual uint32 to_big(uint32 value) = 0;
        virtual uint16 to_big(uint16 value) = 0;

        virtual uint64 to_little(uint64 value) = 0;
        virtual uint32 to_little(uint32 value) = 0;
        virtual uint16 to_little(uint16 value) = 0;

        virtual void destroy() = 0;
    };

    class BigEndian : public IEndian
    {
    public:
        virtual uint64 from_big(uint64 value);
        virtual uint32 from_big(uint32 value);
        virtual uint16 from_big(uint16 value);

        virtual uint64 from_little(uint64 value);
        virtual uint32 from_little(uint32 value);
        virtual uint16 from_little(uint16 value);

        virtual uint64 to_big(uint64 value);
        virtual uint32 to_big(uint32 value);
        virtual uint16 to_big(uint16 value);

        virtual uint64 to_little(uint64 value);
        virtual uint32 to_little(uint32 value);
        virtual uint16 to_little(uint16 value);

        virtual void destroy();
    };

    class LittleEndian : public IEndian
    {
    public:
        virtual uint64 from_big(uint64 value);
        virtual uint32 from_big(uint32 value);
        virtual uint16 from_big(uint16 value);

        virtual uint64 from_little(uint64 value);
        virtual uint32 from_little(uint32 value);
        virtual uint16 from_little(uint16 value);

        virtual uint64 to_big(uint64 value);
        virtual uint32 to_big(uint32 value);
        virtual uint16 to_big(uint16 value);

        virtual uint64 to_little(uint64 value);
        virtual uint32 to_little(uint32 value);
        virtual uint16 to_little(uint16 value);

        virtual void destroy();
    };

    // EndianChooser 判断本机是大端还是小端模式
    class EndianChooser
    {
    public:
        EndianChooser();
        ~EndianChooser();

    public:
        IEndian* endian_;
    };

    class Endian
    {
    public:
        static uint64 from_big(uint64 value);
        static uint32 from_big(uint32 value);
        static uint16 from_big(uint16 value);

        static uint64 from_little(uint64 value);
        static uint32 from_little(uint32 value);
        static uint16 from_little(uint16 value);

        static uint64 to_big(uint64 value);
        static uint32 to_big(uint32 value);
        static uint16 to_big(uint16 value);

        static uint64 to_little(uint64 value);
        static uint32 to_little(uint32 value);
        static uint16 to_little(uint16 value);
    };
}

#endif  // __EDNIAN_EX_H__