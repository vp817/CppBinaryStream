// CppBinaryStream - binary stream c++ library implemention.
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

#include <BMLib/BinaryStream.hpp>

BMLib::BinaryStream::BinaryStream(Buffer *buffer, std::size_t position)
	: buffer(buffer), position(position), curr_read_octet(0), curr_bit_read_pos(0), curr_write_octet(0), curr_bit_write_pos(0)
{
}

BMLib::BinaryStream::~BinaryStream()
{
	delete this->buffer;
	this->buffer = nullptr;
	this->position = 0;
}

void BMLib::BinaryStream::rewind()
{
	this->position = 0;
}

void BMLib::BinaryStream::reset(bool auto_realloc, std::size_t alloc_size)
{
	delete this->buffer;
	this->buffer = Buffer::allocate(auto_realloc, alloc_size);
	this->rewind();
	this->resetBitReader();
	this->resetBitWriter();
}

void BMLib::BinaryStream::setBuffer(Buffer *buffer)
{
	delete this->buffer;
	this->buffer = buffer;
}

bool BMLib::BinaryStream::eos()
{
	return this->position >= this->buffer->getSize();
}

void BMLib::BinaryStream::ignoreBytes(std::size_t size)
{
	this->position += size;
}

void BMLib::BinaryStream::resetBitReader()
{
	this->curr_read_octet = 0;
	this->curr_bit_read_pos = 0;
}

void BMLib::BinaryStream::resetBitWriter()
{
	this->curr_write_octet = 0;
	this->curr_read_octet = 0;
}

void BMLib::BinaryStream::setPosition(std::size_t value)
{
	this->position = value;
}

BMLib::Buffer *BMLib::BinaryStream::getBuffer()
{
	return this->buffer;
}

std::size_t BMLib::BinaryStream::getNumOfBytesRead() const
{
	return this->position;
}

BMLib::Buffer *BMLib::BinaryStream::readAligned(std::size_t size)
{
	if (this->eos())
		throw BMLib::exceptions::EndOfStream("Attempted to read past the end of the stream. No more bytes available to read.");
	this->position += size;
	return new Buffer(&this->buffer->getBinary()[this->position - size], size, 0, false, false);
}

std::uint8_t BMLib::BinaryStream::readSingle()
{
	return this->buffer->at(this->position++);
}

void BMLib::BinaryStream::writePadding(std::uint8_t value, std::size_t size)
{
	std::uint8_t *tmp = static_cast<std::uint8_t *>(std::calloc(size, 1));
	if (value != 0)
		std::fill_n(tmp, size, value);
	this->buffer->writeAligned(tmp, size);
	std::free(tmp);
}

void BMLib::BinaryStream::writeBit(bool value, bool skip, bool msb_o)
{
	std::uint8_t bit_value = static_cast<std::uint8_t>(value);
	if (msb_o)
		bit_value <<= (7 - this->curr_bit_write_pos);
	else
		bit_value <<= this->curr_bit_write_pos;
	this->curr_write_octet |= bit_value;
	if (++this->curr_bit_write_pos == 8 || skip) {
		this->write<std::uint8_t>(this->curr_write_octet);
		this->curr_write_octet = 0;
		this->curr_bit_write_pos = 0;
	}
}

void BMLib::BinaryStream::writeOptional(std::optional<std::function<void(BinaryStream *)>> value)
{
	bool func_exists = value.has_value();
	this->write<bool>(func_exists);
	if (func_exists)
		(value.value())(this);
}

BMLib::Buffer *BMLib::BinaryStream::readPadding(std::uint8_t value, std::size_t size)
{
	Buffer *result = this->readAligned(size);
	std::uint8_t *tmp = new std::uint8_t[size];
	std::fill_n(tmp, size, value);

	if (std::memcmp(result->getBinary(), tmp, size) != 0) {
		delete[] tmp;
		throw exceptions::PaddingOutOfRange("Attempted to read padding of a value when there is no padding of that specific value.");
	}

	delete[] tmp;
	return result;
}

bool BMLib::BinaryStream::readBit(bool skip, bool msb_o)
{
	if (this->curr_read_octet < 1 || this->curr_bit_read_pos == 8 || skip) {
		this->curr_read_octet = this->readSingle();
		this->curr_bit_read_pos = 0;
	}
	std::uint8_t bit_value = this->curr_read_octet;
	if (msb_o)
		bit_value >>= (7 - this->curr_bit_read_pos);
	else {
		bit_value >>= this->curr_bit_read_pos;
	}
	++this->curr_bit_read_pos;
	return (bit_value & 0b1) == 0b1;
}

void BMLib::BinaryStream::readOptional(std::optional<std::function<void(BinaryStream *)>> value)
{
	bool func_exists = value.has_value();
	bool has_structure = this->read<bool>();
	if (func_exists && has_structure)
		(value.value())(this);
}

BMLib::Buffer *BMLib::BinaryStream::readRemaining()
{
	return this->readAligned(this->buffer->getSize() - this->position);
}
