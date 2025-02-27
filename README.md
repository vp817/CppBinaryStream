# CppBinaryStream

CppBinaryStream is a C++ library that provides functionality for working with buffers/binary streams.

## Usage

The `BinaryStream` is the main and it is what you are going to mainly use.

The `Buffer` is the holder of your buffer.

The `Ints` contains new integers that you may want to use.

Read the header file of each one of them for further understanding.

## Building

The library uses `CMake` as the build system. To build the library and the tests, follow these steps:

1. Clone the repository
2. Run `cmake .`
3. Run `cmake --build .`

The library will be built as a static library(which can be changed if wanted by modifing the cmake file).

## Building the Tests

The library comes with a set of tests to ensure its correctness. To build the tests using CMake, you can pass the `-DBINARY_STREAM_COMPILE_TESTS=ON` flag.

## Contributing

If you find any issues or have suggestions for improvement, please open an issue or submit a pull request.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).
