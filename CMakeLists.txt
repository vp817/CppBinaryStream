cmake_minimum_required(VERSION 3.20)
project(BinaryStream)
set(CMAKE_CXX_STANDARD 17)

option(BINARY_STREAM_COMPILE_TESTS "compile the tests or not." OFF)
option(BINARY_STREAM_SHARED "compile the library as shared." OFF)

file(GLOB_RECURSE LIB_FILES src/*.cpp)

if(NOT BINARY_STREAM_SHARED)
	add_library(BinaryStream STATIC ${LIB_FILES})
else()
	add_library(BinaryStream SHARED ${LIB_FILES})
endif()

target_include_directories(BinaryStream PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(BINARY_STREAM_COMPILE_TESTS)
	add_executable(BinaryStreamTests ${LIB_FILES} ${PROJECT_SOURCE_DIR}/tests/Tests.cpp)
	target_include_directories(BinaryStreamTests PUBLIC ${PROJECT_SOURCE_DIR}/include)
endif()
