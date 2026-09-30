cmake_minimum_required(VERSION 3.18)
project(igris)

include(GNUInstallDirs)

set(IGRIS_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(IGRIS_TOP_LEVEL ON)
endif()

option(IGRIS_BUILD_TESTS "Build igris tests" ${IGRIS_TOP_LEVEL})
option(IGRIS_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

# Используем CONFIGURE_DEPENDS чтобы cmake автоматически 
# перезапускался при изменении файлов
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
	igris/*.h
)

file(GLOB IGRIS_SOURCES CONFIGURE_DEPENDS
	igris/util/*.cpp 
	igris/series/*.cpp  
	igris/container/*.cpp  
	igris/util/*.c
	igris/dprint/dprint_func_impl.c
	igris/event/*.cpp
	igris/trent/*.cpp
	igris/datastruct/*.cpp
	igris/protocols/gstuff.cpp
    igris/protocols/gstuff_v1/gstuff.c
    igris/protocols/gstuff_v1/autorecv.c
	igris/sync/syslock_mutex.cpp
	igris/sync/mutex.cpp
	igris/sync/semaphore.cpp
	igris/sync/critical_context.c
    igris/string/replace_substrings.c
	igris/string/replace.cpp
	igris/string/hexascii_string.cpp
	igris/string/memmem.c
	igris/shell/*.cpp
	igris/shell/mshell.c
	igris/time/time_posix.cpp
	igris/osinter/wait.cpp
	igris/osinter/wait-linux.cpp
	igris/osinter/ktimer.c
	igris/binreader.c
	igris/block.cpp
	igris/iterator.cpp
	igris/series.cpp
	igris/path/*.cpp
	igris/shell/*.c
	igris/path/path.cpp
)

file(GLOB IGRIS_HEADERS CONFIGURE_DEPENDS
	igris/*.h 
	igris/util/*.h 
)

file(GLOB IGRIS_SOURCES_PLUS CONFIGURE_DEPENDS
	igris/dprint/dprint_stdout.c
)

if (UNIX)
	file(GLOB PLATFORM_SOURCES CONFIGURE_DEPENDS
		igris/osutil/src/posix.cpp
		igris/osutil/realtime.c
		igris/sclonner.cpp
		igris/sync/semaphore.c	
	)
endif()

file(GLOB SOURCES_TEST CONFIGURE_DEPENDS
        ${IGRIS_SOURCES}
        tests/*.cpp
        tests/igris_std/*.cpp
        tests/archive/*.cpp
        tests/archive20/*.cpp
        tests/container/*.cpp
        tests/shell/*.cpp
        tests/series/*.cpp
)
list(REMOVE_ITEM SOURCES_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/igris/osinter/wait-linux.cpp)
list(APPEND SOURCES_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_wait.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/cooperative_threads_test.cpp)

add_library(igris SHARED ${IGRIS_SOURCES} ${PLATFORM_SOURCES} ${IGRIS_SOURCES_PLUS})
find_package(Threads REQUIRED)
target_link_libraries(igris PUBLIC Threads::Threads)
target_compile_features(igris PUBLIC cxx_std_20)
target_include_directories(igris PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(igris PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
if(IGRIS_BUILD_TESTS)
    enable_testing()
    add_executable(igris_test ${SOURCES_TEST})
    target_compile_features(igris_test PRIVATE cxx_std_20)
    target_include_directories(igris_test PRIVATE . tests)
    target_compile_definitions(igris_test PRIVATE IGRIS_FORCE_INTERNAL_SEMAPHORE=1)
    find_package(Threads REQUIRED)
    target_link_libraries(igris_test PRIVATE Threads::Threads)
    add_test(NAME igris_test COMMAND igris_test)
endif()
if(IGRIS_ENABLE_SANITIZERS)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(FATAL_ERROR "IGRIS_ENABLE_SANITIZERS requires GCC or Clang")
    endif()
    foreach(target igris igris_test)
        if(TARGET ${target})
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        endif()
    endforeach()
    if(IGRIS_BUILD_TESTS)
        # LeakSanitizer is incompatible with makecontext/swapcontext tests.
        set_tests_properties(igris_test PROPERTIES ENVIRONMENT "ASAN_OPTIONS=detect_leaks=0")
    endif()
endif()
install(TARGETS igris EXPORT igris-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY igris DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} PATTERN "*HIDE*" EXCLUDE)
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/igris-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/igris-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igris)
install(EXPORT igris-targets NAMESPACE igris:: FILE igris-targets.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igris)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/igris-config.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/igris)
if(NOT IGRIS_TOP_LEVEL)
    set(IGRIS_SOURCES ${IGRIS_SOURCES} ${PLATFORM_SOURCES} ${IGRIS_SOURCES_PLUS} PARENT_SCOPE)
endif()
