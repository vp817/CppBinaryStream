// CppBinaryStream
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

#pragma once

#include <cstdint>
#include "exceptions/EndOfStream.hpp"
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <string>

namespace BMLib
{
	/// The Buffer class.
	/// The public fields should not be touched unless absolutely needed, and that is also the reason they are public.
	class Buffer
	{
	public:
		static constexpr int DEFAULT_ALLOCATION_SIZE = 512;

		// the allocated or not binary data.
		std::uint8_t *binary;
		// the size of the binary data.
		std::size_t size;
		// the writing position (the number of bytes written).
		std::size_t position;
		// whether auto reallocation is enabled.
		bool auto_realloc;
		// whether the binary data is dynamically allocated and if not then it is statically allocated.
		bool dynamic;

		/// \brief Initializes a new Buffer instance.
		///
		/// \param[in] binary The binary data that will be used.
		/// \param[in] size The size of the binary data which must be the same as the allocated binary size.
		/// \param[in] position The writing position.
		/// \param[in] auto_realloc Enables auto memory reallocation.
		/// \param[in] dynamic Whether the binary data is dynamic or not.
		explicit Buffer(std::uint8_t *binary, std::size_t size, std::size_t position = 0, bool auto_realloc = false, bool dynamic = true);

		/// \brief Allocates an unsafe variable-sized buffer.
		///
		/// \param[in] auto_realloc_enabled Enable memeory auto reallocation.
		/// \param[in] alloc_size The size of the binary data.
		///
		/// \return A Buffer object representing the allocated buffer.
		static Buffer *allocate(bool auto_realloc_enabled = true, std::size_t alloc_size = DEFAULT_ALLOCATION_SIZE);

		/// \brief The destructor for the Buffer class, which deallocates the allocated memory.
		~Buffer();

		/// \brief Writes the binary data after the current binary data.
		///
		/// \param[in] in_buffer The binary data to be merged with the current binary data.
		/// \param[in] in_size The size of the binary data to be merged.
		//
		/// \throws std::invalid_argument if the buffer size or position is negative.
		/// \throws Binary::exceptions::EndOfStream if the buffer is at maximum size and auto reallocation is not enabled.
		void writeAligned(std::uint8_t *in_buffer, size_t in_size);

		/// \brief Writes the buffer data into the current buffer.
		///
		/// \param[in] in_buffer The buffer to be written into the current buffer.
		/// \param[in] destroy destroy the in_buffer.
		///
		/// \throws what the other writeAligned throws
		void writeAligned(Buffer *in_buffer, bool destroy = true);

		/// \brief Writes a single byte to the current binary data.
		///
		/// \param[in] value The byte value to write.
		//
		/// \throws std::invalid_argument if the buffer size or position is negative.
		/// \throws Binary::exceptions::EndOfStream if the buffer is at maximum size and auto reallocation is not enabled.
		void writeSingle(std::uint8_t value);

		/// \brief Retrieves a byte from a specific position in the buffer.
		///
		/// \param[in] pos The position to retrieve the byte from.
		/// \return The byte value at the specified position.
		/// \throws std::out_of_range error
		std::uint8_t at(size_t pos);

	private:
		void internalParamsCheck();
		void internalResize(std::size_t value);
	};
}
