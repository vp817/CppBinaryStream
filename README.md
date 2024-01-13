# CppBinaryStream

CppBinaryStream is a C++ library that provides functionality for working with binary streams. The library provides an API for reading and writing various types of binary data, including integers, floats, and varints. The library also provides bit-level access to the data in the binary stream.

## API

The main component of the API is the `BinaryStream` class. This class provides functions for reading and writing binary data to a buffer. The following functions are available for writing data:

- `write(value, big_endian = true)`: Writes a value to the buffer. The byte order is specified by the `big_endian` parameter.
- `writeFloat(value, big_endian = true)`: Writes a floating-point value to the buffer. The byte order is specified by the `big_endian` parameter.
- `writeString(value, big_endian = true)`: Writes a string to the buffer, preceded by a length value of type `T`. The byte order is specified by the `big_endian` parameter.
- `writeStringVarInt(value)`: Writes a string to the buffer, preceded by a varint32 length value.
- `writeVarInt(value)`: Writes a varint value to the buffer.
- `writeZigZag(value)`: Writes a zigzag-encoded integer to the buffer.
- `writePadding(value, size)`: Writes a padding value to the buffer.
- `writeBit(value, skip = false)`: Writes a single bit to the buffer. The `skip` parameter determines whether to skip to the next byte after writing the bit.
- `writeOptional(value)`: Writes an optional value to the buffer.
- `writeBits(value, size, big_endian = true)`: Writes a specified number of bits from a value to the buffer, starting from the most significant bit. The byte order is specified by the `big_endian` parameter.

The following functions are available for reading data:

- `read<T>(big_endian = true)`: Reads a value of type `T` from the buffer. The byte order is specified by the `big_endian` parameter.
- `readFloat(big_endian = true)`: Reads a floating-point value from the buffer. The byte order is specified by the `big_endian` parameter.
- `readString<T>(big_endian = true)`: Reads a string from the buffer, preceded by a length value of type `T`. The byte order is specified by the `big_endian` parameter.
- `readStringVarInt()`: Reads a string from the buffer, preceded by a varint32 length value.
- `readVarInt<T>()`: Reads a varint value of type `T` from the buffer.
- `readZigZag<T>()`: Reads a zigzag-encoded integer of type `T` from the buffer.
- `readPadding(value, size)`: Reads a padding value from the buffer.
- `readBit(skip = false)`: Reads a single bit from the buffer. The `skip` parameter determines whether to skip to the next byte after reading the bit.
- `readOptional(value)`: Reads an optional value from the buffer.
- `readBits<T>(size, big_endian = true)`: Reads a specified number of bits from the buffer, starting from the most significant bit. The byte order is specified by the `big_endian` parameter.

## Building

The library uses `CMake` as the build system. To build the library and the tests, follow these steps:

1. Clone the repository
2. Run `cmake .`
3. Run `cmake --build .`

The library will be built as a shared library, and the tests will be built as an executable.

## Running the Tests

The library comes with a set of tests to ensure its correctness. To build the tests using CMake, you can pass the `-DBINARY_STREAM_COMPILE_TESTS=ON` flag.

```
cmake . -DBINARY_STREAM_COMPILE_TESTS=ON
```

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvement, please open an issue or submit a pull request.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

If you have any questions or need further assistance, feel free to reach out.
