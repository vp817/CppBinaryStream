// CppBinaryStream - A binary stream library implemented in C++.
//
// Copyright (C) 2024  vp817
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
#include <cstdlib>
#include "Buffer.hpp"
#include "exceptions/EndOfStream.hpp"
#include "exceptions/VarIntTooBig.hpp"
#include "exceptions/ZigZagTooBig.hpp"
#include "exceptions/PaddingOutOfRange.hpp"
#include "Ints.hpp"
#include <cmath>
#include <type_traits>
#include <string>
#include <optional>
#include <functional>
#include <algorithm>
#include <cstring>

namespace Binary
{

	class BinaryStream
	{
	protected:
		Buffer *buffer;
		std::size_t position;
		std::uint8_t current_octet;
		std::size_t bit_count;

	public:
		/// \brief Initializes a new instance of BinaryStream class.
		///
		/// \param[in] buffer The buffer to use.
		/// \param[in] position The reading position.
		explicit BinaryStream(Buffer *buffer, std::size_t position);

		/// \brief Destructor for the BinaryStream class.
		/// This deinitializes the buffer and resets the stream to its default state.
		~BinaryStream();

		/// \brief Rewinds the reading position
		void rewind();

		/// \brief Resets the buffer with a new allocated buffer.
		///
		/// \param[in] auto_reallocation Enable automatic reallocation for the new buffer.
		void reset(bool auto_reallocation = false);

		/// \brief Checks if the stream has reached the end of the stream.
		///
		/// \return True if the stream has reached the end of the stream, false otherwise.
		bool eos();

		/// \brief Skips a specified number of bytes in the stream.
		///
		/// \param[in] size The number of bytes to skip.
		void ignoreBytes(std::size_t size);

		/// \brief Use this function to reset the bit after reading
		/// to avoid interference with other read or write operations that is bit-related.
		void nullifyBit();

		/// \brief Sets the current reading position.
		///
		/// \param[in] value The value to set the reading position to.
		void setPosition(std::size_t value);

		/// \brief Retrieves the current buffer.
		///
		/// \return A pointer to the Buffer.
		Buffer *getBuffer();

		/// \brief Retrieves the current reading position.
		///
		/// \return The current reading position as a std::size_t value.
		std::size_t getPosition() const;

		/// \brief Reads aligned binary from the current position in the buffer.
		///
		/// \param[in] size The size of data to read from the buffer.
		///
		/// \return A pointer to the read binary data as a Buffer.
		/// \throws EndOfStream error
		Buffer *readAligned(std::size_t size);

		/// \brief Writes a type based on what the template type is.
		///
		/// \tparam T the type that will be written.
		/// \param[in] value The value to write into the buffer.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		template <typename T>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_array_v<T> && !std::is_floating_point_v<T> || (std::is_same_v<T, uint24_t> || std::is_same_v<T, int24_t>)> write(T value, bool big_endian = true)
		{
			auto size = sizeof(T);

			if (size == 1)
			{
				std::uint8_t byte_to_write[1] = {static_cast<std::uint8_t>(value)};
				this->buffer->writeAligned(byte_to_write, 1);
				return;
			}

			for (std::size_t i = 0; i < size; ++i)
			{
				this->write<std::uint8_t>(static_cast<std::uint8_t>(value >> static_cast<std::uint8_t>((big_endian ? (size - i - 1) : i) << 3)));
			}
		}

		/// \brief Writes a floating-point number based on what the template type is.
		///
		/// \tparam T the type that will be written.
		/// \param[in] value The value to write into the buffer.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		template <typename T>
		std::enable_if_t<std::is_floating_point_v<T>> writeFloat(T value, bool big_endian = true)
		{
			auto size = sizeof(T);
			auto bit_pattern = *reinterpret_cast<std::size_t *>(&value);

			for (std::size_t i = 0; i < size; ++i)
			{
				this->write<std::uint8_t>(static_cast<std::uint8_t>(bit_pattern >> ((big_endian ? (size - i - 1) : i) << 3)));
			}
		}

		/// \brief Writes a string value to the buffer.
		///
		/// \tparam T the type that will be used to write the string length.
		/// \param[in] value The value to write into the buffer.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		template <typename T>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_array_v<T> && !std::is_floating_point_v<T> && !(std::is_same_v<T, uint24_t> || std::is_same_v<T, int24_t>)> writeString(std::string value, bool big_endian = true)
		{
			this->write<T>(static_cast<T>(value.size()), big_endian);
			this->getBuffer()->writeAligned(reinterpret_cast<std::uint8_t *>(const_cast<char *>(value.c_str())), value.size());
		}

		/// \brief Writes a varint string value to the buffer.
		///
		/// \tparam T the type that will be used to write the string length.
		/// \param[in] value The value to write into the buffer.
		template <typename T = std::uint32_t>
		std::enable_if_t<std::is_arithmetic_v<T> && std::is_unsigned_v<T> && !std::is_array_v<T> && !std::is_floating_point_v<T> && !(std::is_same_v<T, uint24_t> || std::is_same_v<T, int24_t>)> writeStringVarInt(std::string value)
		{
			this->writeVarInt<T>(static_cast<T>(value.size()));
			this->getBuffer()->writeAligned(reinterpret_cast<std::uint8_t *>(const_cast<char *>(value.c_str())), value.size());
		}

		/// \brief Writes a varint value to the buffer.
		///
		/// \tparam T the type that will be written.
		/// \param[in] value The value to write into the buffer.
		template <typename T = std::uint32_t>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_floating_point_v<T> && !std::is_array_v<T> && std::is_unsigned_v<T>> writeVarInt(T value)
		{
			for (std::size_t i = 0; i < std::ceil((sizeof(T) << 3) / 7); ++i)
			{
				std::uint8_t to_write = value & 0x7f;

				value >>= 7;

				if (value != 0)
				{
					this->write<uint8_t>(to_write | 0x80);
				}
				else
				{
					this->write<uint8_t>(to_write);
					break;
				}
			}
		}

		/// \brief Writes a zigzag value to the buffer.
		///
		/// \tparam T the type that will be written.
		/// \param[in] value The value to write into the buffer.
		template <typename T = std::int32_t>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_floating_point_v<T> && !std::is_array_v<T> && std::is_signed_v<T>> writeZigZag(T value)
		{
			auto to_write = static_cast<std::make_unsigned_t<T>>(value);

			this->writeVarInt<decltype(to_write)>((to_write << 1) ^ (to_write >> ((sizeof(T) << 3) - 1)));
		}

		/// \brief Writes a padding to the buffer.
		///
		/// \param[in] value The number that will be padded into buffer.
		/// \param[in] size The number of how much the value will be padded.
		void writePadding(std::uint8_t value, std::size_t size);

		/// \brief Writes a bit to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] skip Whether to skip the process of filling it with zeros until its octet.
		void writeBit(bool value, bool skip = false);

		/// \brief Writes an optional value to the buffer.
		///
		/// \param[in] value The function that will be called if the function not nullopt.
		void writeOptional(std::optional<std::function<void(BinaryStream *)>> value);

		/// \brief Writes bits to the buffer (does not work for floating-point types).
		///
		/// \param[in] value The value to write.
		/// \param[in] size The number of bits to write.
		/// \param[in] big_endian Specifies whether to use big endian bit order.
		template <typename T>
		void writeBits(T value, int size, bool big_endian = true)
		{
			for (std::size_t i = 0; i < size; ++i)
			{
				this->writeBit(((value >> (big_endian ? (size - i - 1) : i)) & 1) == 1);
			}
		}

		/// \brief Reads a type based on what the template type is.
		///
		/// \tparam T the type that will be read.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		///
		/// \return The T value read from the buffer.
		template <typename T>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_array_v<T> && !std::is_floating_point_v<T> || (std::is_same_v<T, uint24_t> || std::is_same_v<T, int24_t>), T> read(bool big_endian = true)
		{
			T result = 0;
			auto size = sizeof(T);

			if (size == 1)
			{
				return static_cast<T>(this->readAligned(1)->at(0));
			}

			for (std::size_t i = 0; i < size; ++i)
			{
				result |= this->read<std::uint8_t>() << ((big_endian ? (size - i - 1) : i) << 3);
			}

			return result;
		}

		/// \brief Reads a floating-point number based on what the template type is.
		///
		/// \tparam T the type that will be read.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		///
		/// \return The T value read from the buffer.
		template <typename T>
		std::enable_if_t<std::is_floating_point_v<T>, T> readFloat(bool big_endian = true)
		{
			auto size = sizeof(T);
			auto bit_pattern_buffer = this->readAligned(size);
			std::size_t result = 0;

			for (size_t i = 0; i < size; ++i)
			{
				result |= static_cast<std::size_t>(bit_pattern_buffer->at(i)) << ((big_endian ? (size - i - 1) : i) << 3);
			}

			return *reinterpret_cast<T *>(&result);
		}

		/// \brief Reads a string value based on what the template type is.
		///
		/// \tparam T the type that will be used to read the string length.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		///
		/// \return The string value read from the buffer.
		template <typename T>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_array_v<T> && !std::is_floating_point_v<T> && !(std::is_same_v<T, uint24_t> || std::is_same_v<T, int24_t>), std::string> readString(bool big_endian = true)
		{
			std::uint8_t *value = this->readAligned(this->read<T>(big_endian))->getBinary();
			return std::string(reinterpret_cast<const char *>(value));
		}

		/// \brief Reads a varint string value based on what the template type is.
		///
		/// \tparam T the type that will be used to read the string length.
		///
		/// \return The string value read from the buffer.
		template <typename T = std::uint32_t>
		std::enable_if_t<std::is_arithmetic_v<T> && std::is_unsigned_v<T> && !std::is_array_v<T> && !std::is_floating_point_v<T> && !(std::is_same_v<T, uint24_t> || std::is_same_v<T, int24_t>), std::string> readStringVarInt()
		{
			return std::string(reinterpret_cast<const char *>(this->readAligned(this->readVarInt<T>())->getBinary()));
		}

		/// \brief Reads a varint value from the buffer.
		///
		/// \tparam T the type that will be read.
		///
		/// \return The T value read from the buffer.
		template <typename T = std::uint32_t>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_floating_point_v<T> && !std::is_array_v<T> && std::is_unsigned_v<T>, T> readVarInt()
		{
			std::uint32_t value = 0;

			for (std::size_t i = 0; i < std::ceil((sizeof(T) << 3) / 7) * 7; i += 7)
			{
				std::uint8_t to_read = this->read<std::uint8_t>();

				value |= (to_read & 0x7f) << i;

				if ((to_read & 0x80) == 0)
				{
					return value;
				}
			}

			throw exceptions::VarIntTooBig("Attempted to decode VarInt that is too big to be represented.");
		}

		/// \brief Reads a varint value from the buffer.
		///
		/// \tparam T the type that will be read.
		///
		/// \return The T value read from the buffer.
		/// \throws ZigZagTooBig error
		template <typename T = std::int32_t>
		std::enable_if_t<std::is_arithmetic_v<T> && !std::is_floating_point_v<T> && !std::is_array_v<T> && std::is_signed_v<T>, T> readZigZag()
		{
			std::make_unsigned_t<T> varint = 0;

			try
			{
				varint = this->readVarInt<decltype(varint)>();
			}
			catch (...)
			{
				throw exceptions::ZigZagTooBig("Attempted to decode ZigZag that is too big to be represented.");
			}

			auto value = static_cast<T>(varint);

			return (value >> 1) ^ -(value & 1);
		}

		/// \brief Reads a padding from the buffer.
		///
		/// \param[in] value The number that was padded into buffer.
		/// \param[in] size The number of how much the value was padded.
		///
		/// \return A Buffer instance representing the padded values.
		Buffer *readPadding(std::uint8_t value, std::size_t size);

		/// \brief Reads a bit from the buffer.
		///
		/// \param[in] skip Whether to skip to a new octet without waiting until the bit is completely read.
		///
		/// \return The boolean value of the bit read from the buffer.
		bool readBit(bool skip = false);

		/// \brief Reads an optional value to the buffer.
		///
		/// \param[in] value The function that will be called if the optional value is found.
		void readOptional(std::optional<std::function<void(BinaryStream *)>> value);

		/// \brief Reads bits from the buffer.
		///
		/// \param[in] size The number of bits to read.
		/// \param[in] big_endian Specifies whether to use big endian byte order.
		///
		/// \return The value read from the buffer as type T.
		template <typename T>
		T readBits(T size, bool big_endian = true)
		{
			T result = 0;

			for (std::size_t i = 0; i < size; ++i)
			{
				result |= static_cast<std::uint8_t>(this->readBit()) << (big_endian ? (size - i - 1) : i);
			}

			return result;
		}

		/// \brief Reads the remaining buffer.
		///
		/// \return A pointer to the Buffer object representing the remaining buffer.
		Buffer *readRemaining();
	};
}
