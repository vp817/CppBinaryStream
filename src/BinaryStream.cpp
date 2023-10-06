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

#include <BinaryStream/BinaryStream.h>

Binary::BinaryStream::~BinaryStream()
{
	delete this->buffer;
	this->position = 0;
}

void Binary::BinaryStream::rewind()
{
	this->position = 0;
}

void Binary::BinaryStream::reset(bool autoReallocation)
{
	delete this->buffer;
	this->buffer = Buffer::allocateZero(autoReallocation);
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
	auto *binary = static_cast<uint8_t *>(calloc(size, 1));

	this->buffer->writeAligned(binary, size);

	delete[] binary;
}

void Binary::BinaryStream::nullifyBit()
{
	if (this->currentOctet != 0 && this->bitCount != 0)
	{
		this->currentOctet = 0;
		this->bitCount = 0;
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

void Binary::BinaryStream::writeUInt8(uint8_t value)
{
	uint8_t bytes[1] = {
		static_cast<uint8_t>(value),
	};

	this->buffer->writeAligned(bytes, 1);
}

void Binary::BinaryStream::writeInt8(int8_t value)
{
	this->writeUInt8(static_cast<uint8_t>(value));
}

void Binary::BinaryStream::writeBool(bool value)
{
	this->writeUInt8(static_cast<uint8_t>(value));
}

void Binary::BinaryStream::writeBit(bool value, bool skip)
{
	if (this->currentOctet != 0 && this->bitCount == 0)
	{
		this->currentOctet = 0;
	}

	this->currentOctet |= static_cast<uint8_t>(value) << (7 - this->bitCount++);

	if (this->bitCount == 8 || skip)
	{
		this->writeUInt8(this->currentOctet);
		this->currentOctet = 0;
		this->bitCount = 0;
	}
}

void Binary::BinaryStream::writeUInt16(uint16_t value, bool bigEndian)
{
	uint8_t bytes[2] = {0};

	if (bigEndian)
	{
		bytes[0] = static_cast<uint8_t>(value >> 8);
		bytes[1] = static_cast<uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<uint8_t>(value);
		bytes[1] = static_cast<uint8_t>(value >> 8);
	}

	this->buffer->writeAligned(bytes, 2);
}

void Binary::BinaryStream::writeInt16(int16_t value, bool bigEndian)
{
	this->writeUInt16(static_cast<uint16_t>(value), bigEndian);
}

void Binary::BinaryStream::writeUInt24(uint32_t value, bool bigEndian)
{
	uint8_t bytes[3] = {0};

	if (bigEndian)
	{
		bytes[0] = static_cast<uint8_t>(value >> 16);
		bytes[1] = static_cast<uint8_t>(value >> 8);
		bytes[2] = static_cast<uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<uint8_t>(value);
		bytes[1] = static_cast<uint8_t>(value >> 8);
		bytes[2] = static_cast<uint8_t>(value >> 16);
	}

	this->buffer->writeAligned(bytes, 3);
}

void Binary::BinaryStream::writeInt24(int32_t value, bool bigEndian)
{
	this->writeUInt24(static_cast<uint32_t>(value < 0x7fffff ? 0x800000 + value : value > 0x7fffff ? value - 0x800000
																								   : value),
					  bigEndian);
}

void Binary::BinaryStream::writeUInt32(uint32_t value, bool bigEndian)
{
	uint8_t bytes[4] = {0};

	if (bigEndian)
	{
		bytes[0] = static_cast<uint8_t>(value >> 24);
		bytes[1] = static_cast<uint8_t>(value >> 16);
		bytes[2] = static_cast<uint8_t>(value >> 8);
		bytes[3] = static_cast<uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<uint8_t>(value);
		bytes[1] = static_cast<uint8_t>(value >> 8);
		bytes[2] = static_cast<uint8_t>(value >> 16);
		bytes[3] = static_cast<uint8_t>(value >> 24);
	}

	this->buffer->writeAligned(bytes, 4);
}

void Binary::BinaryStream::writeInt32(int32_t value, bool bigEndian)
{
	this->writeUInt32(static_cast<uint32_t>(value), bigEndian);
}

void Binary::BinaryStream::writeUInt64(uint64_t value, bool bigEndian)
{
	uint8_t bytes[8] = {0};

	if (bigEndian)
	{
		bytes[0] = static_cast<uint8_t>(value >> 56);
		bytes[1] = static_cast<uint8_t>(value >> 48);
		bytes[2] = static_cast<uint8_t>(value >> 40);
		bytes[3] = static_cast<uint8_t>(value >> 32);
		bytes[4] = static_cast<uint8_t>(value >> 24);
		bytes[5] = static_cast<uint8_t>(value >> 16);
		bytes[6] = static_cast<uint8_t>(value >> 8);
		bytes[7] = static_cast<uint8_t>(value);
	}
	else
	{
		bytes[0] = static_cast<uint8_t>(value);
		bytes[1] = static_cast<uint8_t>(value >> 8);
		bytes[2] = static_cast<uint8_t>(value >> 16);
		bytes[3] = static_cast<uint8_t>(value >> 24);
		bytes[4] = static_cast<uint8_t>(value >> 32);
		bytes[5] = static_cast<uint8_t>(value >> 40);
		bytes[6] = static_cast<uint8_t>(value >> 48);
		bytes[7] = static_cast<uint8_t>(value >> 56);
	}

	this->buffer->writeAligned(bytes, 8);
}

void Binary::BinaryStream::writeInt64(int64_t value, bool bigEndian)
{
	this->writeUInt64(static_cast<uint64_t>(value), bigEndian);
}

void Binary::BinaryStream::writeFloat(float value, bool bigEndian)
{
	uint32_t bitPattern = *reinterpret_cast<uint32_t *>(&value);
	this->writeUInt32(bitPattern, bigEndian);
}

void Binary::BinaryStream::writeDouble(double value, bool bigEndian)
{
	uint64_t bitPattern = *reinterpret_cast<uint64_t *>(&value);
	this->writeUInt64(bitPattern, bigEndian);
}

void Binary::BinaryStream::writeVarInt32(uint32_t value)
{
	for (size_t i = 0; i < 5; ++i)
	{
		uint8_t toWrite = value & 0x7f;

		value >>= 7;

		if (value != 0)
		{
			this->writeUInt8(toWrite | 0x80);
		}
		else
		{
			this->writeUInt8(toWrite);
			break;
		}
	}
}

void Binary::BinaryStream::writeVarInt64(uint64_t value)
{
	for (size_t i = 0; i < 10; ++i)
	{
		uint8_t toWrite = value & 0x7f;

		value >>= 7;

		if (value != 0)
		{
			this->writeUInt8(toWrite | 0x80);
		}
		else
		{
			this->writeUInt8(toWrite);
			break;
		}
	}
}

void Binary::BinaryStream::writeZigZag32(int32_t value)
{
	auto toWrite = static_cast<uint32_t>(value);
	this->writeVarInt32((toWrite << 1) ^ (toWrite >> 31));
}

void Binary::BinaryStream::writeZigZag64(int64_t value)
{
	auto toWrite = static_cast<uint64_t>(value);
	this->writeVarInt64((toWrite << 1) ^ (toWrite >> 63));
}

uint8_t Binary::BinaryStream::readUInt8()
{
	return this->readAligned(1)->at(0);
}

int8_t Binary::BinaryStream::readInt8()
{
	return static_cast<int8_t>(this->readUInt8());
}

bool Binary::BinaryStream::readBool()
{
	return this->readUInt8() == 1;
}

bool Binary::BinaryStream::readBit(bool skip)
{
	if (this->currentOctet != 0 && this->bitCount == 0)
	{
		this->currentOctet = 0;
	}

	if (this->bitCount == 0 || skip)
	{
		this->currentOctet = this->readUInt8();
		this->bitCount = 8;
	}

	return (this->currentOctet & (1 << (--this->bitCount))) != 0;
}

uint16_t Binary::BinaryStream::readUInt16(bool bigEndian)
{
	Buffer *binary = this->readAligned(2);

	return bigEndian
			   ? (
					 static_cast<uint16_t>(binary->at(0) << 8) |
					 static_cast<uint16_t>(binary->at(1)))
			   : (
					 static_cast<uint16_t>(binary->at(0)) |
					 static_cast<uint16_t>(binary->at(1) << 8));
}

int16_t Binary::BinaryStream::readInt16(bool bigEndian)
{
	return static_cast<int16_t>(this->readUInt16(bigEndian));
}

uint32_t Binary::BinaryStream::readUInt24(bool bigEndian)
{
	Buffer *binary = this->readAligned(3);

	return bigEndian
			   ? (
					 static_cast<uint32_t>(binary->at(0) << 16) |
					 static_cast<uint32_t>(binary->at(1) << 8) |
					 static_cast<uint32_t>(binary->at(2))) &
					 0xffffff
			   : (
					 static_cast<uint32_t>(binary->at(0)) |
					 static_cast<uint32_t>(binary->at(1) << 8) |
					 static_cast<uint32_t>(binary->at(2) << 16)) &
					 0xffffff;
}

int32_t Binary::BinaryStream::readInt24(bool bigEndian)
{
	auto result = static_cast<int32_t>(this->readUInt24(bigEndian));

	return result < 0x7fffff ? 0x800000 + result : result > 0x7fffff ? result - 0x800000
																	 : result;
}

uint32_t Binary::BinaryStream::readUInt32(bool bigEndian)
{
	Buffer *binary = this->readAligned(4);

	return bigEndian
			   ? (
					 static_cast<uint32_t>(binary->at(0)) << 24 |
					 static_cast<uint32_t>(binary->at(1) << 16) |
					 static_cast<uint32_t>(binary->at(2) << 8) |
					 static_cast<uint32_t>(binary->at(3)))
			   : (
					 static_cast<uint32_t>(binary->at(0)) |
					 static_cast<uint32_t>(binary->at(1) << 8) |
					 static_cast<uint32_t>(binary->at(2) << 16) |
					 static_cast<uint32_t>(binary->at(3) << 24));
}

int32_t Binary::BinaryStream::readInt32(bool bigEndian)
{
	return static_cast<int32_t>(this->readUInt32(bigEndian));
}

uint64_t Binary::BinaryStream::readUInt64(bool bigEndian)
{
	Buffer *binary = this->readAligned(8);

	return bigEndian
			   ? (static_cast<uint64_t>(binary->at(0)) << 56 |
				  static_cast<uint64_t>(binary->at(1)) << 48 |
				  static_cast<uint64_t>(binary->at(2)) << 40 |
				  static_cast<uint64_t>(binary->at(3)) << 32 |
				  static_cast<uint64_t>(binary->at(4)) << 24 |
				  static_cast<uint64_t>(binary->at(5)) << 16 |
				  static_cast<uint64_t>(binary->at(6)) << 8 |
				  static_cast<uint64_t>(binary->at(7)))
			   : (static_cast<uint64_t>(binary->at(0)) |
				  static_cast<uint64_t>(binary->at(1)) << 8 |
				  static_cast<uint64_t>(binary->at(2)) << 16 |
				  static_cast<uint64_t>(binary->at(3)) << 24 |
				  static_cast<uint64_t>(binary->at(4)) << 32 |
				  static_cast<uint64_t>(binary->at(5)) << 40 |
				  static_cast<uint64_t>(binary->at(6)) << 48 |
				  static_cast<uint64_t>(binary->at(7)) << 56);
}

int64_t Binary::BinaryStream::readInt64(bool bigEndian)
{
	return static_cast<int64_t>(this->readUInt64(bigEndian));
}

float Binary::BinaryStream::readFloat(bool bigEndian)
{
	uint32_t bitPattern = this->readUInt32(bigEndian);
	return *reinterpret_cast<float *>(&bitPattern);
}

double Binary::BinaryStream::readDouble(bool bigEndian)
{
	uint64_t bitPattern = this->readUInt64(bigEndian);
	return *reinterpret_cast<double *>(&bitPattern);
}

uint32_t Binary::BinaryStream::readVarInt32()
{
	uint32_t value = 0;

	for (size_t i = 0; i < 35; i += 7)
	{
		uint8_t toRead = this->readUInt8();

		value |= (toRead & 0x7f) << i;

		if ((toRead & 0x80) == 0)
		{
			return value;
		}
	}

	throw exceptions::VarIntTooBig("Attempted to decode VarInt32 that is too big to be represented.");
}

uint64_t Binary::BinaryStream::readVarInt64()
{
	uint64_t value = 0;

	for (size_t i = 0; i < 70; i += 7)
	{
		uint8_t toRead = this->readUInt8();

		value |= (toRead & 0x7f) << i;

		if ((toRead & 0x80) == 0)
		{
			return value;
		}
	}

	throw exceptions::VarIntTooBig("Attempted to decode VarInt32 that is too big to be represented.");
}

int32_t Binary::BinaryStream::readZigZag32()
{
	auto value = static_cast<int32_t>(this->readVarInt32());

	return (value >> 1) ^ -(value & 1);
}

int64_t Binary::BinaryStream::readZigZag64()
{
	auto value = static_cast<int64_t>(this->readVarInt64());

	return (value >> 1) ^ -(value & 1);
}

Binary::Buffer *Binary::BinaryStream::readRemaining()
{
	return this->readAligned(this->buffer->getSize() - this->position);
}
