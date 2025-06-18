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

#include <BinaryStream/Buffer.hpp>

BMLib::Buffer::Buffer(std::uint8_t *binary, std::size_t size, std::size_t position, bool auto_realloc, bool dynamic)
	: binary(binary), size(size), position(position), auto_realloc(auto_realloc), dynamic(dynamic)
{
}

BMLib::Buffer::~Buffer()
{
	if (this->dynamic)
		std::free(this->binary);
	this->binary = nullptr;
	this->size = -1;
	this->position = -1;
}

BMLib::Buffer *BMLib::Buffer::allocate(bool auto_realloc_enabled, std::size_t alloc_size)
{
	return new Buffer(static_cast<std::uint8_t *>(std::malloc(alloc_size)), alloc_size, 0, auto_realloc_enabled);
}

std::uint8_t *BMLib::Buffer::getBinary()
{
	return this->binary;
}

std::size_t BMLib::Buffer::getSize() const
{
	return this->size;
}

std::size_t BMLib::Buffer::getPosition() const
{
	return this->position;
}

bool BMLib::Buffer::isAutoReallocEnabled() const
{
	return this->auto_realloc;
}

void BMLib::Buffer::writeAligned(std::uint8_t *binary_to_align, std::size_t align_size)
{
	this->internalParamsCheck();
	this->internalResize(align_size);
	this->position += align_size;
	std::memcpy(&this->binary[this->position - align_size], binary_to_align, align_size);
}

void BMLib::Buffer::writeSingle(std::uint8_t value)
{
	this->internalParamsCheck();
	this->internalResize(1);
	this->binary[this->position++] = value;
}

std::uint8_t BMLib::Buffer::at(std::size_t pos)
{
	if (this->size < pos)
		throw std::out_of_range("Attempted to access byte at position " + std::to_string(pos) + ", but buffer size is only " + std::to_string(this->size) + " bytes.");
	return this->binary[pos];
}

void BMLib::Buffer::internalParamsCheck()
{
	if (!this->dynamic)
		throw std::invalid_argument("Attempted to modify non-dynamic buffer.");
	if (this->size < 0 || this->position < 0)
		throw std::invalid_argument("Buffer size and position must not be negative, but got size = " + std::to_string(this->size) + ", position = " + std::to_string(this->position));
}

void BMLib::Buffer::internalResize(std::size_t value)
{
	std::size_t new_size = this->size + value;
	if (new_size > this->size || this->position > this->size) {
		if (this->auto_realloc) {
			this->size = new_size;
			this->binary = static_cast<std::uint8_t *>(std::realloc(this->binary, this->size));
		}
		else
			throw exceptions::EndOfStream("Attempted to write to buffer at position " + std::to_string(this->position) + ", but buffer is at maximum size.");
	}
}
