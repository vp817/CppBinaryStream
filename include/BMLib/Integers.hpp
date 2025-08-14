// CppBinaryStream
//
// Copyright (C) 2025  vp817
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <cstdint>
#include "exceptions/EndOfStream.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <ostream>

namespace BMLib
{
    struct uint24_t
    {
        std::uint8_t bytes[3];

        uint24_t(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2)
        {
            bytes[0] = b0;
            bytes[1] = b1;
            bytes[2] = b2;
        }

        uint24_t(std::uint32_t value = 0)
        {
            bytes[0] = static_cast<std::uint8_t>(value >> 16);
            bytes[1] = static_cast<std::uint8_t>(value >> 8);
            bytes[2] = static_cast<std::uint8_t>(value);
        }

        operator std::uint32_t() const
        {
            std::uint32_t value = 0;
            value |= bytes[0] << 16;
            value |= bytes[1] << 8;
            value |= bytes[2];
            return value;
        }

        uint24_t &operator=(std::uint32_t rhs)
        {
            bytes[0] = static_cast<std::uint8_t>(rhs >> 16);
            bytes[1] = static_cast<std::uint8_t>(rhs >> 8);
            bytes[2] = static_cast<std::uint8_t>(rhs);
            return *this;
        }

        uint24_t &operator++()
        {
            *this += uint24_t(0, 0, 1);
            return *this;
        }

        uint24_t operator++(int)
        {
            uint24_t temp(*this);
            ++(*this);
            return temp;
        }

        uint24_t &operator--()
        {
            *this -= uint24_t(0, 0, 1);
            return *this;
        }

        uint24_t operator--(int)
        {
            uint24_t temp(*this);
            --(*this);
            return temp;
        }

        uint24_t operator+(const uint24_t &rhs) const
        {
            return uint24_t(*this) += rhs;
        }

        uint24_t &operator+=(const uint24_t &rhs)
        {
            std::uint32_t sum = static_cast<std::uint32_t>(*this) + static_cast<std::uint32_t>(rhs);
            *this = sum;
            return *this;
        }

        uint24_t operator-(const uint24_t &rhs) const
        {
            return uint24_t(*this) -= rhs;
        }

        uint24_t &operator-=(const uint24_t &rhs)
        {
            std::uint32_t diff = static_cast<std::uint32_t>(*this) - static_cast<std::uint32_t>(rhs);
            *this = diff;
            return *this;
        }

        uint24_t operator^(const uint24_t &rhs) const
        {
            return uint24_t(*this) ^= rhs;
        }

        uint24_t &operator^=(const uint24_t &rhs)
        {
            bytes[0] ^= rhs.bytes[0];
            bytes[1] ^= rhs.bytes[1];
            bytes[2] ^= rhs.bytes[2];
            return *this;
        }

        uint24_t operator&(const uint24_t &rhs) const
        {
            return uint24_t(*this) &= rhs;
        }

        uint24_t &operator&=(const uint24_t &rhs)
        {
            bytes[0] &= rhs.bytes[0];
            bytes[1] &= rhs.bytes[1];
            bytes[2] &= rhs.bytes[2];
            return *this;
        }

        uint24_t operator|(const uint24_t &rhs) const
        {
            return uint24_t(*this) |= rhs;
        }

        uint24_t &operator|=(const uint24_t &rhs)
        {
            bytes[0] |= rhs.bytes[0];
            bytes[1] |= rhs.bytes[1];
            bytes[2] |= rhs.bytes[2];
            return *this;
        }

        uint24_t operator<<(std::int32_t rhs) const
        {
            return uint24_t(*this) <<= rhs;
        }

        uint24_t &operator<<=(std::int32_t rhs)
        {
            std::uint32_t value = static_cast<std::uint32_t>(*this) << rhs;
            *this = value;
            return *this;
        }

        uint24_t operator>>(std::int32_t rhs) const
        {
            return uint24_t(*this) >>= rhs;
        }

        uint24_t &operator>>=(std::int32_t rhs)
        {
            std::uint32_t value = static_cast<std::uint32_t>(*this) >> rhs;
            *this = value;
            return *this;
        }

        bool operator==(const uint24_t &rhs) const
        {
            return static_cast<std::uint32_t>(*this) == static_cast<std::uint32_t>(rhs);
        }

        bool operator!=(const uint24_t &rhs) const
        {
            return !(*this == rhs);
        }

        bool operator<(const uint24_t &rhs) const
        {
            return static_cast<std::uint32_t>(*this) < static_cast<std::uint32_t>(rhs);
        }

        bool operator>(const uint24_t &rhs) const
        {
            return rhs < *this;
        }

        bool operator<=(const uint24_t &rhs) const
        {
            return !(*this > rhs);
        }

        bool operator>=(const uint24_t &rhs) const
        {
            return !(*this < rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const uint24_t &value)
        {
            os << static_cast<std::uint32_t>(value);
            return os;
        }
    };

    struct int24_t
    {
        std::int8_t bytes[3];

        int24_t(std::int8_t b0, std::int8_t b1, std::int8_t b2)
        {
            bytes[0] = b0;
            bytes[1] = b1;
            bytes[2] = b2;
        }

        int24_t(std::int32_t value = 0)
        {
            bytes[0] = static_cast<std::int8_t>(value >> 16);
            bytes[1] = static_cast<std::int8_t>(value >> 8);
            bytes[2] = static_cast<std::int8_t>(value);
        }

        operator std::int32_t() const
        {
            std::int32_t value = 0;
            value |= bytes[0] << 16;
            value |= bytes[1] << 8;
            value |= bytes[2];
            return value;
        }

        int24_t &operator=(std::int32_t rhs)
        {
            bytes[0] = static_cast<std::int8_t>(rhs >> 16);
            bytes[1] = static_cast<std::int8_t>(rhs >> 8);
            bytes[2] = static_cast<std::int8_t>(rhs);
            return *this;
        }

        int24_t &operator++()
        {
            *this += int24_t(0, 0, 1);
            return *this;
        }

        int24_t operator++(int)
        {
            int24_t temp(*this);
            ++(*this);
            return temp;
        }

        int24_t &operator--()
        {
            *this -= int24_t(0, 0, 1);
            return *this;
        }

        int24_t operator--(int)
        {
            int24_t temp(*this);
            --(*this);
            return temp;
        }

        int24_t operator+(const int24_t &rhs) const
        {
            return int24_t(*this) += rhs;
        }

        int24_t &operator+=(const int24_t &rhs)
        {
            std::int32_t sum = static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(rhs);
            *this = sum;
            return *this;
        }

        int24_t operator-(const int24_t &rhs) const
        {
            return int24_t(*this) -= rhs;
        }

        int24_t &operator-=(const int24_t &rhs)
        {
            std::int32_t diff = static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(rhs);
            *this = diff;
            return *this;
        }

        int24_t operator^(const int24_t &rhs) const
        {
            return int24_t(*this) ^= rhs;
        }

        int24_t &operator^=(const int24_t &rhs)
        {
            bytes[0] ^= rhs.bytes[0];
            bytes[1] ^= rhs.bytes[1];
            bytes[2] ^= rhs.bytes[2];
            return *this;
        }

        int24_t operator&(const int24_t &rhs) const
        {
            return int24_t(*this) &= rhs;
        }

        int24_t &operator&=(const int24_t &rhs)
        {
            bytes[0] &= rhs.bytes[0];
            bytes[1] &= rhs.bytes[1];
            bytes[2] &= rhs.bytes[2];
            return *this;
        }

        int24_t operator|(const int24_t &rhs) const
        {
            return int24_t(*this) |= rhs;
        }

        int24_t &operator|=(const int24_t &rhs)
        {
            bytes[0] |= rhs.bytes[0];
            bytes[1] |= rhs.bytes[1];
            bytes[2] |= rhs.bytes[2];
            return *this;
        }

        int24_t operator<<(std::int32_t rhs) const
        {
            return int24_t(*this) <<= rhs;
        }

        int24_t &operator<<=(std::int32_t rhs)
        {
            std::int32_t value = static_cast<std::int32_t>(*this) << rhs;
            *this = value;
            return *this;
        }

        int24_t operator>>(std::int32_t rhs) const
        {
            return int24_t(*this) >>= rhs;
        }

        int24_t &operator>>=(std::int32_t rhs)
        {
            std::int32_t value = static_cast<std::int32_t>(*this) >> rhs;
            *this = value;
            return *this;
        }

        bool operator==(const int24_t &rhs) const
        {
            return static_cast<std::int32_t>(*this) == static_cast<std::int32_t>(rhs);
        }

        bool operator!=(const int24_t &rhs) const
        {
            return !(*this == rhs);
        }

        bool operator<(const int24_t &rhs) const
        {
            return static_cast<std::int32_t>(*this) < static_cast<std::int32_t>(rhs);
        }

        bool operator>(const int24_t &rhs) const
        {
            return rhs < *this;
        }

        bool operator<=(const int24_t &rhs) const
        {
            return !(*this > rhs);
        }

        bool operator>=(const int24_t &rhs) const
        {
            return !(*this < rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const int24_t &value)
        {
            os << static_cast<std::int32_t>(value);
            return os;
        }
    };
}
