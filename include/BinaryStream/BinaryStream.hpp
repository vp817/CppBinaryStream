// CppBinaryStream - A binary stream library implemented in C++.
//
// Copyright (C) 2023  vp817
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

namespace Binary
{

	/// \brief The BinaryStream class
	class BinaryStream
	{
	protected:
		Buffer *buffer;
		size_t position;
		uint8_t currentOctet;
		size_t bitCount;

	public:
		/// \brief Initializes a new instance of BinaryStream class.
		///
		/// \param[in] buffer The buffer to use.
		/// \param[in] position The reading position.
		explicit BinaryStream(Buffer *buffer, size_t position);

		/// \brief Destructor for the BinaryStream class.
		/// This deinitializes the buffer and resets the stream to its default state.
		~BinaryStream();

		/// \brief Rewinds the reading position
		void rewind();

		/// \brief Resets the buffer with a new allocated buffer.
		///
		/// \param[in] autoReallocation Enable automatic reallocation for the new buffer.
		void reset(bool autoReallocation = false);

		/// \brief Checks if the stream has reached the end of the stream.
		///
		/// \return True if the stream has reached the end of the stream, false otherwise.
		bool eos();

		/// \brief Skips a specified number of bytes in the stream.
		///
		/// \param[in] size The number of bytes to skip.
		void ignoreBytes(size_t size);

		/// \brief Writes zero padding to the buffer.
		///
		/// \param[in] size The number of zeros to pad.
		void padBufferWithZero(size_t size);

		/// \brief Use this function to reset the bit after reading
		/// to avoid interference with other read or write operations that is bit-related.
		void nullifyBit();

		/// \brief Sets the current reading position.
		///
		/// \param[in] value The value to set the reading position to.
		void setPosition(size_t value);

		/// \brief Retrieves the current buffer.
		///
		/// \return A pointer to the Buffer.
		Buffer *getBuffer();

		/// \brief Retrieves the current reading position.
		///
		/// \return The current reading position as a size_t value.
		size_t getPosition() const;

		/// \brief Reads aligned binary from the current position in the buffer.
		///
		/// \param[in] size The size of data to read from the buffer.
		///
		/// \return A pointer to the read binary data as a Buffer.
		/// \throws EndOfStream error
		Buffer *readAligned(size_t size);

		/// \brief Writes a uint8 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeUInt8(uint8_t value);

		/// \brief Writes an int8 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeInt8(int8_t value);

		/// \brief Writes a boolean value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeBool(bool value);

		/// \brief Writes a bit to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] skip Whether to skip the process of filling it with zeros until its octet.
		void writeBit(bool value, bool skip = false);

		/// \brief Writes bits to the buffer (does not work for floating-point types).
		///
		/// \param[in] value The value to write.
		/// \param[in] size The number of bits to write.
		/// \param[in] bigEndian Specifies whether to use big endian bit order.
		template <typename T>
		void writeBits(T value, int size, bool bigEndian = true)
		{
			for (size_t i = 0; i < size; ++i)
			{
				this->writeBit(((value >> (bigEndian ? (size - i - 1) : i)) & 1) == 1);
			}
		}

		/// \brief Writes a uint16 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeUInt16(uint16_t value, bool bigEndian = true);

		/// \brief Writes an int16 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeInt16(int16_t value, bool bigEndian = true);

		/// \brief Writes a uint24 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeUInt24(uint32_t value, bool bigEndian = true);

		/// \brief Writes an int24 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeInt24(int32_t value, bool bigEndian = true);

		/// \brief Writes a uint32 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeUInt32(uint32_t value, bool bigEndian = true);

		/// \brief Writes an int32 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeInt32(int32_t value, bool bigEndian = true);

		/// \brief Writes a uint64 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeUInt64(uint64_t value, bool bigEndian = true);

		/// \brief Writes an int64 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeInt64(int64_t value, bool bigEndian = true);

		/// \brief Writes a float value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeFloat(float value, bool bigEndian = true);

		/// \brief Writes a double value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		void writeDouble(double value, bool bigEndian = true);

		/// \brief Writes a varint32 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeVarInt32(uint32_t value);

		/// \brief Writes a varint64 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeVarInt64(uint64_t value);

		/// \brief Writes a zigzag32 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeZigZag32(int32_t value);

		/// \brief Writes a zigzag64 value to the buffer.
		///
		/// \param[in] value The value to write into the buffer.
		void writeZigZag64(int64_t value);

		/// \brief Reads a uint8 value from the buffer.
		///
		/// \return The uint8 value read from the buffer.
		uint8_t readUInt8();

		/// \brief Reads an int8 value from the buffer.
		///
		/// \return The int8 value read from the buffer.
		int8_t readInt8();

		/// \brief Reads a boolean value from the buffer.
		///
		/// \return The boolean value read from the buffer.
		bool readBool();

		/// \brief Reads a bit from the buffer.
		///
		/// \param[in] skip Whether to skip to a new octet without waiting until the bit is completely read.
		/// \return The boolean value of the bit read from the buffer.
		bool readBit(bool skip = false);

		/// \brief Reads bits from the buffer.
		///
		/// \param[in] size The number of bits to read.
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		/// \return The value read from the buffer as type T.
		template <typename T>
		T readBits(T size, bool bigEndian = true)
		{
			T result = 0;

			for (size_t i = 0; i < size; ++i)
			{
				result |= static_cast<uint8_t>(this->readBit()) << (bigEndian ? (size - i - 1) : i);
			}

			return result;
		}

		/// \brief Reads a uint16 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The uint16 value read from the buffer.
		uint16_t readUInt16(bool bigEndian = true);

		/// \brief Reads an int16 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The int16 value read from the buffer.
		int16_t readInt16(bool bigEndian = true);

		/// \brief Reads a uint24 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The uint32 value read from the buffer, limited to the range of uint24.
		uint32_t readUInt24(bool bigEndian = true);

		/// \brief Reads an int24 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The int32 value read from the buffer, limited to the range of int24.
		int32_t readInt24(bool bigEndian = true);

		/// \brief Reads a uint32 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The uint32 value read from the buffer.
		uint32_t readUInt32(bool bigEndian = true);

		/// \brief Reads an int32 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The int32 value read from the buffer.
		int32_t readInt32(bool bigEndian = true);

		/// \brief Reads a uint64 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The uint64 value read from the buffer.
		uint64_t readUInt64(bool bigEndian = true);

		/// \brief Reads an int64 value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The int64 value read from the buffer.
		int64_t readInt64(bool bigEndian = true);

		/// \brief Reads a float value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The float value read from the buffer.
		float readFloat(bool bigEndian = true);

		/// \brief Reads a double value from the buffer.
		///
		/// \param[in] bigEndian Specifies whether to use big endian byte order.
		///
		/// \return The double value read from the buffer.
		double readDouble(bool bigEndian = true);

		/// \brief Reads a varint32 value from the buffer and returns a uint32_t.
		///
		/// \return The uint32_t value read from the buffer.
		///
		/// \throws VarIntTooBig error
		uint32_t readVarInt32();

		/// \brief Reads a varint64 value from the buffer and returns a uint64_t.
		///
		/// \return The uint64_t value read from the buffer.
		/// \throws VarIntTooBig error
		uint64_t readVarInt64();

		/// \brief Reads a zigzag32 value from the buffer and returns an int32_t.
		///
		/// \return The int32_t value read from the buffer.
		/// \throws VarIntTooBig error
		int32_t readZigZag32();

		/// \brief Reads a zigzag64 value from the buffer and returns an int64_t.
		///
		/// \return The int64_t value read from the buffer.
		/// \throws VarIntTooBig error
		int64_t readZigZag64();

		/// \brief Reads the remaining buffer.
		///
		/// \return A pointer to the Buffer object representing the remaining buffer.
		Buffer *readRemaining();
	};
}
