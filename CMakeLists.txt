cmake_minimum_required(VERSION 3.0.2)
project(mpiex CXX)
enable_testing()

include(CTest)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY tests)

find_package(MPI REQUIRED)

if(NOT MSVC)
	add_definitions(-Wall -std=c++1y)
else()
	foreach(flagvar CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
	    CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		string(REGEX REPLACE "/MD" "/MT" ${flagvar} "${${flagvar}}")
		string(REGEX REPLACE "/MDd" "/MTd" ${flagvar} "${${flagvar}}")
	endforeach()
endif()

include_directories(include)

file(GLOB mpiex_srcs src/*.cc src/*.cpp)
file(GLOB tests_srcs tests/*.cc tests/*.t.cpp)

add_library(mpiex STATIC ${mpiex_srcs})

target_include_directories(mpiex PUBLIC ${MPI_INCLUDE_PATH})
target_link_libraries(mpiex ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
	if(NOT MSVC)
		set_target_properties(mpiex PROPERTIES COMPILE_FLAGS
		    "-Wconversion -Wno-sign-conversion ${MPI_COMPILE_FLAGS}")
	else()
		set_target_properties(mpiex PROPERTIES COMPILE_FLAGS
		    "${MPI_COMPILE_FLAGS}")
	endif()
endif()

if(MPI_LINK_FLAGS)
	set_target_properties(mpiex PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

if(BUILD_TESTING)
	foreach(test_src ${tests_srcs})
		get_filename_component(test_suite ${test_src} NAME_WE)
		add_executable(${test_suite} ${test_src})
		target_link_libraries(${test_suite} mpiex)
		add_test(${test_suite} ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 5
            tests/${test_suite})
	endforeach()
endif()
