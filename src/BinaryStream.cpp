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

#include <BinaryStream/BinaryStream.hpp>

Binary::BinaryStream::BinaryStream(Buffer *buffer, size_t position)
	: buffer(buffer), position(position), current_octet(0), bit_count(0)
{
}

Binary::BinaryStream::~BinaryStream()
{
	delete this->buffer;
	this->buffer = nullptr;
	this->position = 0;
}

void Binary::BinaryStream::rewind()
{
	this->position = 0;
}

void Binary::BinaryStream::reset(bool auto_reallocation)
{
	delete this->buffer;
	this->buffer = Buffer::allocateZero(auto_reallocation);
	this->rewind();
}

bool Binary::BinaryStream::eos()
{
	return this->position >= this->buffer->getSize();
}

void Binary::BinaryStream::ignoreBytes(size_t size)
{
	this->position += size;
}

void Binary::BinaryStream::padBufferWithZero(size_t size)
{
	auto *binary = static_cast<std::uint8_t *>(calloc(size, 1));

	this->buffer->writeAligned(binary, size);

	delete[] binary;
}

void Binary::BinaryStream::nullifyBit()
{
	if (this->current_octet != 0 && this->bit_count != 0)
	{
		this->current_octet = 0;
		this->bit_count = 0;
	}
}

void Binary::BinaryStream::setPosition(size_t value)
{
	this->position = value;
}

Binary::Buffer *Binary::BinaryStream::getBuffer()
{
	return this->buffer;
}

size_t Binary::BinaryStream::getPosition() const
{
	return this->position;
}

Binary::Buffer *Binary::BinaryStream::readAligned(size_t size)
{
	if (this->eos())
	{
		throw Binary::exceptions::EndOfStream("Attempted to read past the end of the stream. No more bytes are available to read.");
	}

	this->position += size;
	return new Buffer(&this->buffer->getBinary()[this->position - size], size);
}

void Binary::BinaryStream::writeUInt8(std::uint8_t value)
{
	std::uint8_t bytes[1] = {
		static_cast<std::uint8_t>(value),
	};

	this->buffer->writeAligned(bytes, 1);
}

void Binary::BinaryStream::writeInt8(std::int8_t value)
{
	this->writeUInt8(static_cast<std::uint8_t>(value));
}

void Binary::BinaryStream::writeBool(bool value)
{
	this->writeUInt8(static_cast<std::uint8_t>(value));
}

void Binary::BinaryStream::writeBit(bool value, bool skip)
{
	if (this->current_octet != 0 && this->bit_count == 0)
	{
		this->current_octet = 0;
	}

	this->current_octet |= static_cast<std::uint8_t>(value) << (7 - this->bit_count++);

	if (this->bit_count == 8 || skip)
	{
		this->writeUInt8(this->current_octet);
		this->current_octet = 0;
		this->bit_count = 0;
	}
}

void Binary::BinaryStream::writeUInt16(std::uint16_t value, bool big_endian)
{
	std::uint8_t bytes[2] = {0};

	if (big_endian)
	{
		bytes[0] = static_cast<std::uint8_t>(value >> 8);
		bytes[1] = static_cast<std::uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<std::uint8_t>(value);
		bytes[1] = static_cast<std::uint8_t>(value >> 8);
	}

	this->buffer->writeAligned(bytes, 2);
}

void Binary::BinaryStream::writeInt16(std::int16_t value, bool big_endian)
{
	this->writeUInt16(static_cast<std::uint16_t>(value), big_endian);
}

void Binary::BinaryStream::writeUInt24(std::uint32_t value, bool big_endian)
{
	std::uint8_t bytes[3] = {0};

	if (big_endian)
	{
		bytes[0] = static_cast<std::uint8_t>(value >> 16);
		bytes[1] = static_cast<std::uint8_t>(value >> 8);
		bytes[2] = static_cast<std::uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<std::uint8_t>(value);
		bytes[1] = static_cast<std::uint8_t>(value >> 8);
		bytes[2] = static_cast<std::uint8_t>(value >> 16);
	}

	this->buffer->writeAligned(bytes, 3);
}

void Binary::BinaryStream::writeInt24(std::int32_t value, bool big_endian)
{
	this->writeUInt24(static_cast<std::uint32_t>(value < 0x7fffff ? 0x800000 + value : value > 0x7fffff ? value - 0x800000
																								   : value),
					  big_endian);
}

void Binary::BinaryStream::writeUInt32(std::uint32_t value, bool big_endian)
{
	std::uint8_t bytes[4] = {0};

	if (big_endian)
	{
		bytes[0] = static_cast<std::uint8_t>(value >> 24);
		bytes[1] = static_cast<std::uint8_t>(value >> 16);
		bytes[2] = static_cast<std::uint8_t>(value >> 8);
		bytes[3] = static_cast<std::uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<std::uint8_t>(value);
		bytes[1] = static_cast<std::uint8_t>(value >> 8);
		bytes[2] = static_cast<std::uint8_t>(value >> 16);
		bytes[3] = static_cast<std::uint8_t>(value >> 24);
	}

	this->buffer->writeAligned(bytes, 4);
}

void Binary::BinaryStream::writeInt32(std::int32_t value, bool big_endian)
{
	this->writeUInt32(static_cast<std::uint32_t>(value), big_endian);
}

void Binary::BinaryStream::writeUInt64(std::uint64_t value, bool big_endian)
{
	std::uint8_t bytes[8] = {0};

	if (big_endian)
	{
		bytes[0] = static_cast<std::uint8_t>(value >> 56);
		bytes[1] = static_cast<std::uint8_t>(value >> 48);
		bytes[2] = static_cast<std::uint8_t>(value >> 40);
		bytes[3] = static_cast<std::uint8_t>(value >> 32);
		bytes[4] = static_cast<std::uint8_t>(value >> 24);
		bytes[5] = static_cast<std::uint8_t>(value >> 16);
		bytes[6] = static_cast<std::uint8_t>(value >> 8);
		bytes[7] = static_cast<std::uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<std::uint8_t>(value);
		bytes[1] = static_cast<std::uint8_t>(value >> 8);
		bytes[2] = static_cast<std::uint8_t>(value >> 16);
		bytes[3] = static_cast<std::uint8_t>(value >> 24);
		bytes[4] = static_cast<std::uint8_t>(value >> 32);
		bytes[5] = static_cast<std::uint8_t>(value >> 40);
		bytes[6] = static_cast<std::uint8_t>(value >> 48);
		bytes[7] = static_cast<std::uint8_t>(value >> 56);
	}

	this->buffer->writeAligned(bytes, 8);
}

void Binary::BinaryStream::writeInt64(std::int64_t value, bool big_endian)
{
	this->writeUInt64(static_cast<std::uint64_t>(value), big_endian);
}

void Binary::BinaryStream::writeFloat(float value, bool big_endian)
{
	std::uint32_t bitPattern = *reinterpret_cast<std::uint32_t *>(&value);
	this->writeUInt32(bitPattern, big_endian);
}

void Binary::BinaryStream::writeDouble(double value, bool big_endian)
{
	std::uint64_t bitPattern = *reinterpret_cast<std::uint64_t *>(&value);
	this->writeUInt64(bitPattern, big_endian);
}

void Binary::BinaryStream::writeVarInt32(std::uint32_t value)
{
	for (size_t i = 0; i < 5; ++i)
	{
		std::uint8_t to_write = value & 0x7f;

		value >>= 7;

		if (value != 0)
		{
			this->writeUInt8(to_write | 0x80);
		}
		else
		{
			this->writeUInt8(to_write);
			break;
		}
	}
}

void Binary::BinaryStream::writeVarInt64(std::uint64_t value)
{
	for (size_t i = 0; i < 10; ++i)
	{
		std::uint8_t to_write = value & 0x7f;

		value >>= 7;

		if (value != 0)
		{
			this->writeUInt8(to_write | 0x80);
		}
		else
		{
			this->writeUInt8(to_write);
			break;
		}
	}
}

void Binary::BinaryStream::writeZigZag32(std::int32_t value)
{
	auto to_write = static_cast<std::uint32_t>(value);

	this->writeVarInt32((to_write << 1) ^ (to_write >> 31));
}

void Binary::BinaryStream::writeZigZag64(std::int64_t value)
{
	auto to_write = static_cast<std::uint64_t>(value);

	this->writeVarInt64((to_write << 1) ^ (to_write >> 63));
}

std::uint8_t Binary::BinaryStream::readUInt8()
{
	return this->readAligned(1)->at(0);
}

std::int8_t Binary::BinaryStream::readInt8()
{
	return static_cast<std::int8_t>(this->readUInt8());
}

bool Binary::BinaryStream::readBool()
{
	return this->readUInt8() == 1;
}

bool Binary::BinaryStream::readBit(bool skip)
{
	if (this->current_octet != 0 && this->bit_count == 0)
	{
		this->current_octet = 0;
	}

	if (this->bit_count == 0 || skip)
	{
		this->current_octet = this->readUInt8();
		this->bit_count = 8;
	}

	return (this->current_octet & (1 << (--this->bit_count))) != 0;
}

std::uint16_t Binary::BinaryStream::readUInt16(bool big_endian)
{
	Buffer *binary = this->readAligned(2);

	return big_endian
			   ? (
					 static_cast<std::uint16_t>(binary->at(0) << 8) |
					 static_cast<std::uint16_t>(binary->at(1)))
			   : (
					 static_cast<std::uint16_t>(binary->at(0)) |
					 static_cast<std::uint16_t>(binary->at(1) << 8));
}

std::int16_t Binary::BinaryStream::readInt16(bool big_endian)
{
	return static_cast<std::int16_t>(this->readUInt16(big_endian));
}

std::uint32_t Binary::BinaryStream::readUInt24(bool big_endian)
{
	Buffer *binary = this->readAligned(3);

	return big_endian
			   ? (
					 static_cast<std::uint32_t>(binary->at(0) << 16) |
					 static_cast<std::uint32_t>(binary->at(1) << 8) |
					 static_cast<std::uint32_t>(binary->at(2))) &
					 0xffffff
			   : (
					 static_cast<std::uint32_t>(binary->at(0)) |
					 static_cast<std::uint32_t>(binary->at(1) << 8) |
					 static_cast<std::uint32_t>(binary->at(2) << 16)) &
					 0xffffff;
}

std::int32_t Binary::BinaryStream::readInt24(bool big_endian)
{
	auto result = static_cast<std::int32_t>(this->readUInt24(big_endian));

	return result < 0x7fffff ? 0x800000 + result : result > 0x7fffff ? result - 0x800000
																	 : result;
}

std::uint32_t Binary::BinaryStream::readUInt32(bool big_endian)
{
	Buffer *binary = this->readAligned(4);

	return big_endian
			   ? (
					 static_cast<std::uint32_t>(binary->at(0)) << 24 |
					 static_cast<std::uint32_t>(binary->at(1) << 16) |
					 static_cast<std::uint32_t>(binary->at(2) << 8) |
					 static_cast<std::uint32_t>(binary->at(3)))
			   : (
					 static_cast<std::uint32_t>(binary->at(0)) |
					 static_cast<std::uint32_t>(binary->at(1) << 8) |
					 static_cast<std::uint32_t>(binary->at(2) << 16) |
					 static_cast<std::uint32_t>(binary->at(3) << 24));
}

std::int32_t Binary::BinaryStream::readInt32(bool big_endian)
{
	return static_cast<std::int32_t>(this->readUInt32(big_endian));
}

std::uint64_t Binary::BinaryStream::readUInt64(bool big_endian)
{
	Buffer *binary = this->readAligned(8);

	return big_endian
			   ? (static_cast<std::uint64_t>(binary->at(0)) << 56 |
				  static_cast<std::uint64_t>(binary->at(1)) << 48 |
				  static_cast<std::uint64_t>(binary->at(2)) << 40 |
				  static_cast<std::uint64_t>(binary->at(3)) << 32 |
				  static_cast<std::uint64_t>(binary->at(4)) << 24 |
				  static_cast<std::uint64_t>(binary->at(5)) << 16 |
				  static_cast<std::uint64_t>(binary->at(6)) << 8 |
				  static_cast<std::uint64_t>(binary->at(7)))
			   : (static_cast<std::uint64_t>(binary->at(0)) |
				  static_cast<std::uint64_t>(binary->at(1)) << 8 |
				  static_cast<std::uint64_t>(binary->at(2)) << 16 |
				  static_cast<std::uint64_t>(binary->at(3)) << 24 |
				  static_cast<std::uint64_t>(binary->at(4)) << 32 |
				  static_cast<std::uint64_t>(binary->at(5)) << 40 |
				  static_cast<std::uint64_t>(binary->at(6)) << 48 |
				  static_cast<std::uint64_t>(binary->at(7)) << 56);
}

std::int64_t Binary::BinaryStream::readInt64(bool big_endian)
{
	return static_cast<std::int64_t>(this->readUInt64(big_endian));
}

float Binary::BinaryStream::readFloat(bool big_endian)
{
	std::uint32_t bit_pattern = this->readUInt32(big_endian);

	return *reinterpret_cast<float *>(&bit_pattern);
}

double Binary::BinaryStream::readDouble(bool big_endian)
{
	std::uint64_t bit_pattern = this->readUInt64(big_endian);

	return *reinterpret_cast<double *>(&bit_pattern);
}

std::uint32_t Binary::BinaryStream::readVarInt32()
{
	std::uint32_t value = 0;

	for (size_t i = 0; i < 35; i += 7)
	{
		std::uint8_t to_read = this->readUInt8();

		value |= (to_read & 0x7f) << i;

		if ((to_read & 0x80) == 0)
		{
			return value;
		}
	}

	throw exceptions::VarIntTooBig("Attempted to decode VarInt32 that is too big to be represented.");
}

std::uint64_t Binary::BinaryStream::readVarInt64()
{
	std::uint64_t value = 0;

	for (size_t i = 0; i < 70; i += 7)
	{
		std::uint8_t to_read = this->readUInt8();

		value |= (to_read & 0x7f) << i;

		if ((to_read & 0x80) == 0)
		{
			return value;
		}
	}

	throw exceptions::VarIntTooBig("Attempted to decode VarInt32 that is too big to be represented.");
}

std::int32_t Binary::BinaryStream::readZigZag32()
{
	auto value = static_cast<std::int32_t>(this->readVarInt32());

	return (value >> 1) ^ -(value & 1);
}

std::int64_t Binary::BinaryStream::readZigZag64()
{
	auto value = static_cast<std::int64_t>(this->readVarInt64());

	return (value >> 1) ^ -(value & 1);
}

Binary::Buffer *Binary::BinaryStream::readRemaining()
{
	return this->readAligned(this->buffer->getSize() - this->position);
}
