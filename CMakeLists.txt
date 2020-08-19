cmake_minimum_required(VERSION 3.5.1)
project(igris)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CC_STANDARD 11)

set (PLATFORM_SOURCES )

if (WIN32 OR WIN64)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

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
	igris/protocols/gstuff.c
	igris/sync/syslock_mutex.cpp
)

if (UNIX)
	set(PLATFORM_SOURCES 
		igris/osutil/src/posix.cpp
	)
endif()

file(GLOB SOURCES_TEST 
	igris/util/*.cpp 
	igris/util/*.c
	igris/dprint/dprint_manually.c
	igris/dprint/dprint_func_impl.c
	igris/event/*.cpp
	igris/datastruct/*.cpp
	igris/protocols/gstuff/*.c
	igris/sync/syslock_mutex.cpp
	tests/*.cpp
)

add_library(igris SHARED ${SOURCES} ${PLATFORM_SOURCES})
target_include_directories(igris PRIVATE .)

add_executable(igris_test ${SOURCES_TEST})
target_include_directories(igris_test PRIVATE .)
target_link_libraries(igris_test)

if (UNIX AND NOT APPLE)
	install(TARGETS ${PROJECT_NAME}
		LIBRARY DESTINATION /usr/lib
	)

	install(DIRECTORY ${PROJECT_NAME}
    	    DESTINATION /usr/local/include
    	    PATTERN "*HIDE*" EXCLUDE
   	)
endif()


if (APPLE)
	install(TARGETS ${PROJECT_NAME}
		LIBRARY DESTINATION /usr/local/lib
	)

	install(DIRECTORY ${PROJECT_NAME}
    	    DESTINATION /usr/local/include
    	    PATTERN "*HIDE*" EXCLUDE
   	)
endif()

enable_testing()
add_test(NAME igris_test COMMAND igris_test)
