cmake_minimum_required(VERSION 3.12)
project(igris)

include(GNUInstallDirs)

set(IGRIS_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(IGRIS_TOP_LEVEL ON)
endif()

option(IGRIS_BUILD_TESTS "Build igris tests" ${IGRIS_TOP_LEVEL})

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
        container/*.cpp
        shell/*.cpp
        series/*.cpp
)
list(REMOVE_ITEM SOURCES_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/igris/osinter/wait-linux.cpp)
list(APPEND SOURCES_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_wait.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/cooperative_threads_test.cpp)

if(IGRIS_TOP_LEVEL)

	set(CMAKE_VERBOSE_MAKEFILE on )
	set(CMAKE_C_STANDARD 11)
	set(CMAKE_CXX_STANDARD 20)
	
	if (${STATIC_LIBS})
		set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++")
	endif()
	
	if (WIN32 OR WIN64)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	endif()
	
        add_library(igris SHARED ${IGRIS_SOURCES} ${PLATFORM_SOURCES} igris/dprint/dprint_stdout.c)
        target_include_directories(igris
			PUBLIC
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		)
		
		if (IGRIS_BUILD_TESTS)
	                add_executable(igris_test ${SOURCES_TEST})
	                target_include_directories(igris_test PRIVATE . tests)
	                target_compile_definitions(igris_test PRIVATE IGRIS_FORCE_INTERNAL_SEMAPHORE=1)
			#target_link_libraries(igris_test rt pthread)
			target_link_libraries(igris_test pthread)
			#target_link_libraries(igris_test)
		endif()

	#set_target_properties(igris PROPERTIES PUBLIC_HEADER 
	#	"${IGRIS_HEADERS}"
	#)
	
	if (WIN32)	
		install (TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

		install (EXPORT ${PROJECT_NAME}
			NAMESPACE ${PROJECT_NAME}::
			FILE ${PROJECT_NAME}-config.cmake
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	    	    PATTERN "*HIDE*" EXCLUDE
	   	)
	endif()

	if (UNIX AND NOT APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		)
	
		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	    	    PATTERN "*HIDE*" EXCLUDE
	   	)
	endif()
	
	
	if (APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		)
	
		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	    	    PATTERN "*HIDE*" EXCLUDE
	   	)
	endif()
	
	if (IGRIS_BUILD_TESTS)
		enable_testing()
		add_test(NAME igris_test COMMAND igris_test)
	endif()

else()
	set(IGRIS_SOURCES 
		${IGRIS_SOURCES} 
		${PLATFORM_SOURCES} 
		${IGRIS_SOURCES_PLUS} 
		PARENT_SCOPE
	)

endif()
