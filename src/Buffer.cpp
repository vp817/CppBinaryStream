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

#include <BinaryStream/Buffer.hpp>

Binary::Buffer::Buffer(std::uint8_t *binary, std::size_t size, std::size_t position, bool auto_reallocation)
	: binary(binary), size(size), position(position), auto_reallocation(auto_reallocation)
{
}

Binary::Buffer::~Buffer()
{
	delete[] this->binary;
	this->binary = nullptr;
	this->size = -1;
	this->position = -1;
}

Binary::Buffer *Binary::Buffer::allocateZero(bool auto_reallocation_enabled)
{
	return new Buffer(new std::uint8_t[0], 0, 0, auto_reallocation_enabled);
}

std::uint8_t *Binary::Buffer::getBinary()
{
	return this->binary;
}

std::size_t Binary::Buffer::getSize() const
{
	return this->size;
}

std::size_t Binary::Buffer::getPosition() const
{
	return this->position;
}

bool Binary::Buffer::isAutoReallocationEnabled() const
{
	return this->auto_reallocation;
}

void Binary::Buffer::writeAligned(std::uint8_t *binary_to_align, std::size_t align_size)
{
	std::size_t new_size = this->size + align_size;

	if (this->size < 0 || this->position < 0)
	{
		throw std::invalid_argument("Buffer size and position must not be negative, but got size = " + std::to_string(this->size) + ", position = " + std::to_string(this->position));
	}

	if (new_size > this->size || this->position > this->size)
	{
		if (this->auto_reallocation)
		{
			this->size = new_size;
			this->binary = static_cast<std::uint8_t *>(realloc(this->binary, this->size));
		}
		else if (!this->auto_reallocation)
		{
			throw exceptions::EndOfStream("Attempted to write to buffer at position " + std::to_string(this->position) + ", but buffer is at maximum size.");
		}
	}

	this->position += align_size;
	std::memcpy(&this->binary[this->position - align_size], binary_to_align, align_size);
}

std::uint8_t Binary::Buffer::at(std::size_t pos)
{
	if (this->size < pos)
	{
		throw std::out_of_range("Attempted to access byte at position " + std::to_string(pos) + ", but buffer size is only " + std::to_string(this->size) + " bytes.");
	}

	return static_cast<std::uint8_t>(this->binary[pos]);
}
