cmake_minimum_required(VERSION 3.20)
project(BinaryStream)
set(CMAKE_CXX_STANDARD 17)

option(BINARY_STREAM_COMPILE_TESTS "compile the tests or not." OFF)

file(GLOB_RECURSE LIB_FILES src/*.cpp)

add_library(BinaryStream STATIC ${LIB_FILES})
target_include_directories(BinaryStream PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BINARY_STREAM_COMPILE_TESTS)
	file(GLOB TEST_FILES
		tests/*.cpp
	)

	add_executable(BinaryStreamTests ${LIB_FILES} ${TEST_FILES})
	target_include_directories(BinaryStreamTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
