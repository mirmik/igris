cmake_minimum_required(VERSION 3.5.1)
project(igris)

#set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
#set(CMAKE_CXX_COMPILER "C:\\Program Files\\LLVM\\bin\\clang++.exe")

message("${CMAKE_INSTALL_INCLUDEDIR}/igris")

file(GLOB HEADERS 
	igris/util/*.h	
	igris/dprint/*.h
	igris/event/*.h
	igris/datastruct/*.h
)

file(GLOB IGRIS_SOURCES 
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
	igris/sync/semaphore.cpp
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

file(GLOB IGRIS_HEADERS 
	igris/*.h 
	igris/util/*.h 
)

file(GLOB IGRIS_SOURCES_PLUS 
	igris/dprint/dprint_stdout.c
)

if (UNIX)
	file(GLOB PLATFORM_SOURCES 
		igris/osutil/src/posix.cpp
		igris/osutil/realtime.c
		igris/sclonner.cpp
		igris/sync/semaphore.c	
	)
endif()

file(GLOB SOURCES_TEST 
	${IGRIS_SOURCES}
	tests/*.cpp
	container/*.cpp
	shell/*.cpp
	series/*.cpp
)

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})

	set(CMAKE_VERBOSE_MAKEFILE on )
	set(CMAKE_CC_STANDARD 11)
	set(CMAKE_CXX_STANDARD 20)
	
	if (${STATIC_LIBS})
		set(CMAKE_SHARED_LINKER_FLAGS "-static-libstdc++")
	endif()
	
	if (WIN32 OR WIN64)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	endif()
	
	add_library(igris SHARED ${IGRIS_SOURCES} ${PLATFORM_SOURCES} igris/dprint/dprint_stdout.c)
	target_include_directories(igris PRIVATE .)
	
	if (NOT DEFINED WITHOUT_TESTS)
		add_executable(igris_test ${SOURCES_TEST})
		target_include_directories(igris_test PRIVATE . tests)
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
		LIBRARY DESTINATION /)

		install (EXPORT ${PROJECT_NAME}
			NAMESPACE ${PROJECT_NAME}::
			FILE ${PROJECT_NAME}-config.cmake
			DESTINATION lib/cmake/${PROJECT_NAME})

		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION "C:/Program Files (x86)/${PROJECT_NAME}/include"
	    	    PATTERN "*HIDE*" EXCLUDE
	   	)
	endif()

	if (UNIX AND NOT APPLE)
		install(TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION /usr/lib
		)
	
		install(DIRECTORY ${PROJECT_NAME}
	    	    DESTINATION /usr/local/include/
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

else()
	set(IGRIS_SOURCES 
		${IGRIS_SOURCES} 
		${PLATFORM_SOURCES} 
		${IGRIS_SOURCES_PLUS} 
		PARENT_SCOPE
	)

endif()
