# CppBinaryStream

CppBinaryStream is a C++ library that provides functionality for working with binary streams. The library provides an API for reading and writing various types of binary data, including integers, floats, and varints. The library also provides bit-level access to the data in the binary stream.

## API

The main component of the API is the `BinaryStream` class. This class provides functions for reading and writing binary data to a buffer. The following functions are available for writing data:

- `writeUInt8(uint8_t value)`: Writes an 8-bit unsigned integer to the buffer.
- `writeInt8(int8_t value)`: Writes an 8-bit signed integer to the buffer.
- `writeBool(bool value)`: Writes a boolean value to the buffer.
- `writeBit(bool value)`: Writes a single bit to the buffer.
- `writeBits(T value, int size)`: Writes a specified number of bits from a value to the buffer.
- `writeUInt16(uint16_t value)`: Writes a 16-bit unsigned integer to the buffer.
- `writeInt16(int16_t value)`: Writes a 16-bit signed integer to the buffer.
- `writeUInt24(uint32_t value)`: Writes a 24-bit unsigned integer to the buffer.
- `writeInt24(int32_t value)`: Writes a 24-bit signed integer to the buffer.
- `writeUInt32(uint32_t value)`: Writes a 32-bit unsigned integer to the buffer.
- `writeInt32(int32_t value)`: Writes a 32-bit signed integer to the buffer.
- `writeUInt64(uint64_t value)`: Writes a 64-bit unsigned integer to the buffer.
- `writeInt64(int64_t value)`: Writes a 64-bit signed integer to the buffer.
- `writeFloat(float value)`: Writes a floating-point value to the buffer.
- `writeDouble(double value)`: Writes a double-precision floating-point value to the buffer.
- `writeVarInt32(uint32_t value)`: Writes a varint32 value to the buffer.
- `writeVarInt64(uint64_t value)`: Writes a varint64 value to the buffer.
- `writeZigZag32(int32_t value)`: Writes a zigzag-encoded 32-bit integer to the buffer.
- `writeZigZag64(int64_t value)`: Writes a zigzag-encoded 64-bit integer to the buffer.

The following functions are available for reading data:

- `readUInt8()`: Reads an 8-bit unsigned integer from the buffer.
- `readInt8()`: Reads an 8-bit signed integer from the buffer.
- `readBool()`: Reads a boolean value from the buffer.
- `readBit()`: Reads a single bit from the buffer.
- `readBits(T size)`: Reads a specified number of bits from the buffer and returns a value of type T.
- `readUInt16()`: Reads a 16-bit unsigned integer from the buffer.
- `readInt16()`: Reads a 16-bit signed integer from the buffer.
- `readUInt24()`: Reads a 24-bit unsigned integer from the buffer.
- `readInt24()`: Reads a 24-bit signed integer from the buffer.
- `readUInt32()`: Reads a 32-bit unsigned integer from the buffer.
- `readInt32()`: Reads a 32-bit signed integer from the buffer.
- `readUInt64()`: Reads a 64-bit unsigned integer from the buffer.
- `readInt64()`: Reads a 64-bit signed integer from the buffer.
- `readFloat()`: Reads a floating-point value from the buffer.
- `readDouble()`: Reads a double-precision floating-point value from the buffer.
- `readVarInt32()`: Reads a varint32 value from the buffer and returns a uint32_t.
- `readVarInt64()`: Reads a varint64 value from the buffer and returns a uint64_t.
- `readZigZag32()`: Reads a zigzag-encoded 32-bit integer from the buffer and returns an int32_t.
- `readZigZag64()`: Reads a zigzag-encoded 64-bit integer from the buffer and returns an int64_t.
- `readRemaining()`: Reads the remaining data in the buffer and returns a pointer to a `Buffer` object.

The `BinaryStream` class also provides functions for other operations, including skipping bytes, padding the buffer with zeros, and resetting the buffer.

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
