cmake_minimum_required(VERSION 3.5.1)
project(igris)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CC_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set (PLATFORM_SOURCES )

set(CMAKE_INSTALL_RPATH "\$ORIGIN")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if (${STATIC_LIBS})
	set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++")
endif()

if (WIN32 OR WIN64)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
endif()

file(GLOB HEADERS 
	igris/util/*.h	
	igris/dprint/*.h
	igris/event/*.h
	igris/datastruct/*.h
)

file(GLOB IGRIS_SOURCES 
	igris/util/*.cpp 
	igris/util/*.c
	igris/dprint/dprint_func_impl.c
	igris/event/*.cpp
	igris/trent/*.cpp
	igris/datastruct/*.cpp
	igris/sync/semaphore.c
	igris/protocols/gstuff.c
	igris/protocols/gstuff_v1/gstuff.c
	igris/protocols/gstuff_v1/autorecv.c
	igris/sync/syslock_mutex.cpp
	igris/string/replace_substrings.c
	igris/string/replace.cpp
	igris/string/hexascii_string.cpp
	igris/string/memmem.c
	igris/shell/mshell.c
	igris/time/time_posix.cpp
	igris/osinter/wait.c
	igris/osinter/wait-linux.cpp
	igris/osinter/ktimer.c
	igris/osutil/realtime.c
	igris/binreader.c
	igris/block.cpp
	igris/iterator.cpp
	igris/series.cpp
	igris/sclonner.cpp
	igris/path/*.cpp
	igris/shell/*.c
	igris/path/path.cpp
)

file(GLOB IGRIS_SOURCES_PLUS 
	igris/dprint/dprint_stdout.c
)

if (UNIX)
	file(GLOB PLATFORM_SOURCES 
		igris/osutil/src/posix.cpp
	)
endif()

set(IGRIS_SOURCES 
	${IGRIS_SOURCES} 
	${IGRIS_SOURCES_PLUS} 
	${PLATFORM_SOURCES} 
	PARENT_SCOPE
)

file(GLOB SOURCES_TEST 
	${IGRIS_SOURCES}
	tests/*.cpp
	container/*.cpp
	shell/*.cpp
	series/*.cpp
)

add_library(igris SHARED ${IGRIS_SOURCES} ${PLATFORM_SOURCES} igris/dprint/dprint_stdout.c)
target_include_directories(igris PRIVATE .)

#if (!${WITHOUT_TESTS})
	add_executable(igris_test ${SOURCES_TEST})
	target_include_directories(igris_test PRIVATE . tests)
	target_link_libraries(igris_test rt pthread)
#endif()

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
