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

#include <BinaryStream/BinaryStream.hpp>

BMLib::BinaryStream::BinaryStream(Buffer *buffer, std::size_t position)
	: buffer(buffer), position(position), current_octet(0), bit_count(0)
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
	this->buffer = Buffer::allocate(auto_realloc);
	this->rewind();
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

void BMLib::BinaryStream::nullifyBit()
{
	if (this->current_octet > 0 && this->bit_count > 0) {
		this->current_octet = 0;
		this->bit_count = 0;
	}
}

void BMLib::BinaryStream::setPosition(std::size_t value)
{
	this->position = value;
}

BMLib::Buffer *BMLib::BinaryStream::getBuffer()
{
	return this->buffer;
}

std::size_t BMLib::BinaryStream::getPosition() const
{
	return this->position;
}

BMLib::Buffer *BMLib::BinaryStream::readAligned(std::size_t size)
{
	this->internalEosCheck();
	this->position += size;
	return new Buffer(&this->buffer->getBinary()[this->position - size], size, 0, false, false);
}

std::uint8_t BMLib::BinaryStream::readSingle()
{
	this->internalEosCheck();
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

void BMLib::BinaryStream::writeBit(bool value, bool skip)
{
	if (this->current_octet > 0 && this->bit_count == 0)
		this->current_octet = 0;

	this->current_octet |= static_cast<std::uint8_t>(value) << (7 - this->bit_count++);

	if (this->bit_count == 8 || skip) {
		this->write<std::uint8_t>(this->current_octet);
		this->current_octet = 0;
		this->bit_count = 0;
	}
}

void BMLib::BinaryStream::writeOptional(std::optional<std::function<void(BinaryStream *)>> value)
{
	bool exists = value.has_value();
	this->write<bool>(exists);
	if (exists)
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

bool BMLib::BinaryStream::readBit(bool skip)
{
	if (this->current_octet > 0 && this->bit_count == 0)
		this->current_octet = 0;
	if (this->bit_count == 0 || skip) {
		this->current_octet = this->read<std::uint8_t>();
		this->bit_count = 8;
	}
	return (this->current_octet & (1 << (--this->bit_count))) != 0;
}

void BMLib::BinaryStream::readOptional(std::optional<std::function<void(BinaryStream *)>> value)
{
	bool exists = value.has_value();
	bool has_value = this->read<bool>(exists);
	if (exists && has_value)
		(value.value())(this);
}

BMLib::Buffer *BMLib::BinaryStream::readRemaining()
{
	return this->readAligned(this->buffer->getSize() - this->position);
}

void BMLib::BinaryStream::internalEosCheck()
{
	if (this->eos())
		throw BMLib::exceptions::EndOfStream("Attempted to read past the end of the stream. No more bytes available to read.");
}
