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

#include <BinaryStream/Buffer.h>

Binary::Buffer *Binary::Buffer::allocateZero(bool autoReallocationEnabled)
{
	return new Buffer(new uint8_t[0], 0, 0, autoReallocationEnabled);
}

uint8_t *Binary::Buffer::getBinary()
{
	return this->binary;
}

size_t Binary::Buffer::getSize() const
{
	return this->size;
}

size_t Binary::Buffer::getPosition() const
{
	return this->position;
}

bool Binary::Buffer::isAutoReallocationEnabled() const
{
	return this->autoReallocation;
}

void Binary::Buffer::writeAligned(uint8_t *binaryToAlign, size_t alignSize)
{
	size_t newSize = this->size + alignSize;

	if (this->size < 0 || this->position < 0)
	{
		throw std::invalid_argument("Buffer size and position must not be negative, but got size = " + std::to_string(this->size) + ", position = " + std::to_string(this->position));
	}

	if (newSize > this->size || this->position > this->size)
	{
		if (this->autoReallocation)
		{
			this->size = newSize;
			this->binary = static_cast<uint8_t *>(realloc(this->binary, this->size));
		}
		else if (!this->autoReallocation)
		{
			throw exceptions::EndOfStream("Attempted to write to buffer at position " + std::to_string(this->position) + ", but buffer is at maximum size.");
		}
	}

	this->position += alignSize;
	memcpy(&this->binary[this->position - alignSize], binaryToAlign, alignSize);
}

uint8_t Binary::Buffer::at(size_t pos)
{
	if (this->size < pos)
	{
		throw std::out_of_range("Attempted to access byte at position " + std::to_string(pos) + ", but buffer size is only " + std::to_string(this->size) + " bytes.");
	}

	return static_cast<uint8_t>(this->binary[pos]);
}
