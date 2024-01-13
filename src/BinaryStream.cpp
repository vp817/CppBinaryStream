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

#include <BinaryStream/BinaryStream.hpp>

Binary::BinaryStream::BinaryStream(Buffer *buffer, std::size_t position)
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

void Binary::BinaryStream::ignoreBytes(std::size_t size)
{
	this->position += size;
}

void Binary::BinaryStream::nullifyBit()
{
	if (this->current_octet != 0 && this->bit_count != 0)
	{
		this->current_octet = 0;
		this->bit_count = 0;
	}
}

void Binary::BinaryStream::setPosition(std::size_t value)
{
	this->position = value;
}

Binary::Buffer *Binary::BinaryStream::getBuffer()
{
	return this->buffer;
}

std::size_t Binary::BinaryStream::getPosition() const
{
	return this->position;
}

Binary::Buffer *Binary::BinaryStream::readAligned(std::size_t size)
{
	if (this->eos())
	{
		throw Binary::exceptions::EndOfStream("Attempted to read past the end of the stream. No more bytes are available to read.");
	}

	this->position += size;
	return new Buffer(&this->buffer->getBinary()[this->position - size], size);
}

void Binary::BinaryStream::writePadding(std::uint8_t value, std::size_t size)
{
	auto *binary = static_cast<std::uint8_t *>(calloc(size, 1));

	if (value != 0)
	{
		std::fill_n(binary, size, value);
	}

	this->buffer->writeAligned(binary, size);

	delete[] binary;
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
		this->write<std::uint8_t>(this->current_octet);
		this->current_octet = 0;
		this->bit_count = 0;
	}
}

void Binary::BinaryStream::writeOptional(std::optional<std::function<void(BinaryStream *)>> value)
{
	bool exists = value.has_value();

	this->write<bool>(exists);

	if (exists)
	{
		(value.value())(this);
	}
}

Binary::Buffer *Binary::BinaryStream::readPadding(std::uint8_t value, std::size_t size)
{
	Buffer *result = this->readAligned(size);
	auto temp_binary = new std::uint8_t[size];
	std::fill_n(temp_binary, size, value);

	if (std::memcmp(result->getBinary(), temp_binary, size) != 0)
	{
		delete[] temp_binary;
		throw exceptions::PaddingOutOfRange("Attempted to read padding when padding buffer content does not match.");
	}

	delete[] temp_binary;
	return result;
}

bool Binary::BinaryStream::readBit(bool skip)
{
	if (this->current_octet != 0 && this->bit_count == 0)
	{
		this->current_octet = 0;
	}

	if (this->bit_count == 0 || skip)
	{
		this->current_octet = this->read<std::uint8_t>();
		this->bit_count = 8;
	}

	return (this->current_octet & (1 << (--this->bit_count))) != 0;
}

void Binary::BinaryStream::readOptional(std::optional<std::function<void(BinaryStream *)>> value)
{
	bool exists = value.has_value();

	bool has_value = this->read<bool>(exists);

	if (exists && has_value)
	{
		(value.value())(this);
	}
}

Binary::Buffer *Binary::BinaryStream::readRemaining()
{
	return this->readAligned(this->buffer->getSize() - this->position);
}
