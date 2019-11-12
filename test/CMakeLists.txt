cmake_minimum_required(VERSION 3.5.1)
project(igris_test)
set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES 
	*.cpp
)

target_include_directories(igris_test PRIVATE ..)
add_executable(igris_test SHARED ${SOURCES})
