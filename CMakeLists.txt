cmake_minimum_required(VERSION 3.5.1)
project(igris)
set(CMAKE_CXX_STANDARD 14)

file(GLOB HEADERS 
	igris/util/*.h	
	igris/dprint/*.h
	igris/event/*.h
	igris/datastruct/*.h
)

file(GLOB SOURCES 
	igris/util/*.cpp 
	igris/util/*.c
	igris/dprint/dprint_stdout.c
	igris/dprint/dprint_func_impl.c
	igris/event/*.cpp
	igris/datastruct/*.cpp
)

file(GLOB SOURCES_TEST 
	test/*.cpp
)

add_executable(igris_test ${SOURCES_TEST})
add_library(igris SHARED ${SOURCES})

target_include_directories(igris_test PRIVATE .)
target_include_directories(igris PRIVATE .)
target_link_libraries(igris_test igris)
target_link_libraries(igris_test gtest)
target_link_libraries(igris_test pthread)

install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION /usr/lib
	PUBLIC_HEADER DESTINATION /usr/local/include
)

install(DIRECTORY ${PROJECT_NAME}
        DESTINATION /usr/local/include
        PATTERN "*HIDE*" EXCLUDE)

add_test(NAME igris_test COMMAND igris_test)