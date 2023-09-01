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
#include "exceptions/EndOfStream.h"
#include <stdexcept>
#include <cstdlib>
#include <string>

namespace Binary
{

	/// \brief The Buffer class
	class Buffer
	{
	protected:
		/// \brief A pointer to a uint8_t array representing the binary data.
		uint8_t *binary;
		/// \brief The size of the binary data.
		size_t size;
		/// \brief The current position of the write head in the binary data buffer.
		size_t position;
		/// \brief Whether automatic reallocation of the binary data buffer is enabled.
		bool autoReallocation;

	public:
		/// \brief Initializes a new Buffer instance.
		/// \param[in] binary The binary data that will be used.
		/// \param[in] size The size of the binary data (must be the same as the allocated binary size).
		/// \param[in] position The writing position.
		/// \param[in] autoReallocation Enables auto memory reallocation.
		explicit Buffer(uint8_t *binary, size_t size, size_t position = 0, bool autoReallocation = false)
			: binary(binary), size(size), position(position), autoReallocation(autoReallocation)
		{
		}

		/// \brief Allocates a zero-sized buffer with memory reallocation enabled.
		/// \param[in] autoReallocationEnabled Specifies whether auto reallocation is enabled or not.
		/// \return A Buffer object representing the allocated buffer.
		static Buffer *allocateZero(bool autoReallocationEnabled = true);

		/// \brief The destructor for the Buffer object, responsible for memory deallocation and resetting the buffer to its default state.
		~Buffer()
		{
			delete[] this->binary;
			this->size = -1;
			this->position = -1;
		}

		/// \brief Retrieves the current binary data.
		/// \return A pointer to the uint8_t array representing the binary data.
		uint8_t *getBinary();

		/// \brief Retrieves the current size of the binary data.
		/// \return The size of the binary data as a size_t value.
		size_t getSize() const;

		/// \brief Retrieves the current writing position in the binary data.
		/// \return The writing position as a size_t value.
		size_t getPosition() const;

		/// \brief Checks if auto reallocation is enabled.
		/// \return A boolean value indicating whether auto reallocation is enabled or not.
		bool isAutoReallocationEnabled() const;

		/// \brief Writes the binary data after the current binary data.
		/// \param[in] binaryToAlign The binary data to be merged with the current binary data.
		/// \param[in] alignSize The size of the binary data to be merged.
		/// \throws std::invalid_argument if the buffer size or position is negative.
		/// \throws Binary::exceptions::EndOfStream if the buffer is at maximum size and auto reallocation is not enabled.
		void writeAligned(uint8_t *binaryToAlign, size_t alignSize);

		/// \brief Retrieves a byte from a specific position in the buffer.
		/// \param[in] pos The position to retrieve the byte from.
		/// \return The byte value at the specified position.
		/// \throws std::out_of_range error
		uint8_t at(size_t pos);
	};
}
