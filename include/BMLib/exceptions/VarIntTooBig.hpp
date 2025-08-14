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

#include <stdexcept>

namespace BMLib::exceptions
{
	class VarIntTooBig : public std::exception
	{
	public:
		/// \brief Initializes a new VarIntTooBig error to be thrown.
		///
		/// \param[in] value The error message.
		///
		/// \throws VarIntTooBig error
		explicit VarIntTooBig(std::string value) : message(std::string("[VarIntTooBig] ") + std::move(value)) {}
		explicit VarIntTooBig(const char *value) : message(std::string("[VarIntTooBig] ") + value) {}

		/// \brief Retrieves the error message as a string.
		///
		/// \return The error message as a std::string.
		std::string getMessage()
		{
			return message;
		}

		/// \brief Retrieves the exception message to be displayed.
		///
		/// \return A const char* representing the exception message.
		const char *what() const noexcept override
		{
			return message.c_str();
		}

	private:
		std::string message;
	};
}
