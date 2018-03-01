#include "endianEx.h"

namespace pack
{
    /***************************************
     BigEndian
     ***************************************/
    uint64 BigEndian::from_big(uint64 value)
    {
        return value;
    }

    uint32 BigEndian::from_big(uint32 value)
    {
        return value;
    }

    uint16 BigEndian::from_big(uint16 value)
    {
        return value;
    }

    uint64 BigEndian::from_little(uint64 value)
    {
        return (value & 0xFF00000000000000ULL >> 0x38 |
                value & 0x00FF000000000000ULL >> 0x28 |
                value & 0x0000FF0000000000ULL >> 0x18 |
                value & 0x000000FF00000000ULL >> 0x08 |
                value & 0x00000000FF000000ULL << 0x08 |
                value & 0x0000000000FF0000ULL << 0x18 |
                value & 0x000000000000FF00ULL << 0x28 |
                value & 0x00000000000000FFULL << 0x38)
    }

    uint32 BigEndian::from_little(uint32 value)
    {
        return (value & 0xFF000000 >> 0x18 |
                value & 0x00FF0000 >> 0x08 |
                value & 0x0000FF00 << 0x08 |
                value & 0x000000FF << 0x18)
    }

    uint16 BigEndian::from_little(uint16 value)
    {
        return (value & 0xFF00 >> 0x08 |
                value & 0x00FF << 0x08)
    }

    uint64 BigEndian::to_big(uint64 value)
    {
        return value;
    }

    uint32 BigEndian::to_big(uint32 value)
    {
        return value;
    }

    uint16 BigEndian::to_big(uint16 value)
    {
        return value;
    }

    uint64 BigEndian::to_little(uint64 value)
    {
        return (value & 0xFF00000000000000ULL >> 0x38 |
                value & 0x00FF000000000000ULL >> 0x28 |
                value & 0x0000FF0000000000ULL >> 0x18 |
                value & 0x000000FF00000000ULL >> 0x08 |
                value & 0x00000000FF000000ULL << 0x08 |
                value & 0x0000000000FF0000ULL << 0x18 |
                value & 0x000000000000FF00ULL << 0x28 |
                value & 0x00000000000000FFULL << 0x38)
    }

    uint32 BigEndian::to_little(uint32 value)
    {
        return (value & 0xFF000000 >> 0x18 |
                value & 0x00FF0000 >> 0x08 |
                value & 0x0000FF00 << 0x08 |
                value & 0x000000FF << 0x18)
    }

    uint16 BigEndian::to_little(uint16 value)
    {
        return (value & 0xFF00 >> 0x08 |
                value & 0x00FF << 0x08)
    }

    void BigEndian::destroy()
    {
        delete this;
    }

    /***************************************
     LittleEndian
     ***************************************/
    uint64 LittleEndian::from_big(uint64 value)
    {
        return (value & 0xFF00000000000000ULL >> 0x38 |
                value & 0x00FF000000000000ULL >> 0x28 |
                value & 0x0000FF0000000000ULL >> 0x18 |
                value & 0x000000FF00000000ULL >> 0x08 |
                value & 0x00000000FF000000ULL << 0x08 |
                value & 0x0000000000FF0000ULL << 0x18 |
                value & 0x000000000000FF00ULL << 0x28 |
                value & 0x00000000000000FFULL << 0x38)
    }

    uint32 LittleEndian::from_big(uint32 value)
    {
        return (value & 0xFF000000 >> 0x18 |
                value & 0x00FF0000 >> 0x08 |
                value & 0x0000FF00 << 0x08 |
                value & 0x000000FF << 0x18)
    }

    uint16 LittleEndian::from_big(uint16 value)
    {
        return (value & 0xFF00 >> 0x08 |
                value & 0x00FF << 0x08)
    }

    uint64 LittleEndian::from_little(uint64 value)
    {
        return value;
    }

    uint32 LittleEndian::from_little(uint32 value)
    {
        return value;
    }

    uint16 LittleEndian::from_little(uint16 value)
    {
        return value;
    }

    uint64 LittleEndian::to_big(uint64 value)
    {
        return (value & 0xFF00000000000000ULL >> 0x38 |
                value & 0x00FF000000000000ULL >> 0x28 |
                value & 0x0000FF0000000000ULL >> 0x18 |
                value & 0x000000FF00000000ULL >> 0x08 |
                value & 0x00000000FF000000ULL << 0x08 |
                value & 0x0000000000FF0000ULL << 0x18 |
                value & 0x000000000000FF00ULL << 0x28 |
                value & 0x00000000000000FFULL << 0x38)
    }

    uint32 LittleEndian::to_big(uint32 value)
    {
        return (value & 0xFF000000 >> 0x18 |
                value & 0x00FF0000 >> 0x08 |
                value & 0x0000FF00 << 0x08 |
                value & 0x000000FF << 0x18)
    }

    uint16 LittleEndian::to_big(uint16 value)
    {
        return (value & 0xFF00 >> 0x08 |
                value & 0x00FF << 0x08)
    }

    uint64 LittleEndian::to_little(uint64 value)
    {
        return value;
    }

    uint32 LittleEndian::to_little(uint32 value)
    {
        return value;
    }

    uint16 LittleEndian::to_little(uint16 value)
    {
        return value;
    }

    void LittleEndian::destroy()
    {
        delete this;
    }


    /***************************************
     EndianChooser
     ***************************************/
    EndianChooser::EndianChooser()
    {
        unsigned short i = 1;
        if (*((char*)&i))
        {
            endian_ = new LittleEndian();
        }
        else
        {
            endian_ = new BigEndian();
        }
    }

    EndianChooser::~EndianChooser()
    {
        if (NULL != endian_)
        {
            delete endian_;
            endian_ = NULL;
        }
    }


    /***************************************
     Endian
     ***************************************/
    EndianChooser chooser;
    uint64 Endian::from_big(uint64 value)
    {
        return chooser.endian_->from_big(value);
    }

    uint32 Endian::from_big(uint32 value)
    {
        return chooser.endian_->from_big(value);
    }

    uint16 Endian::from_big(uint16 value)
    {
        return chooser.endian_->from_big(value);
    }

    uint64 Endian::from_little(uint64 value)
    {
        return chooser.endian_->from_little(value);
    }

    uint32 Endian::from_little(uint32 value)
    {
        return chooser.endian_->from_little(value);
    }

    uint16 Endian::from_little(uint16 value)
    {
        return chooser.endian_->from_little(value);
    }

    uint64 Endian::to_big(uint64 value)
    {
        return chooser.endian_->to_big(value);
    }

    uint32 Endian::to_big(uint32 value)
    {
        return chooser.endian_->to_big(value);
    }

    uint16 Endian::to_big(uint16 value)
    {
        return chooser.endian_->to_big(value);
    }

    uint64 Endian::to_little(uint64 value)
    {
        return chooser.endian_->to_little(value);
    }

    uint32 Endian::to_little(uint32 value)
    {
        return chooser.endian_->to_little(value);
    }

    uint16 Endian::to_little(uint16 value)
    {
        return chooser.endian_->to_little(value);
    }
}