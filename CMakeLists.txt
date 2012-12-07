#Standard stuff
cmake_minimum_required(VERSION 2.6)
project(libuv_test)

#Check threads
set(CMAKE_THREAD_PREFER_PTHREAD 1)
include(FindThreads)

if(NOT CMAKE_USE_PTHREADS_INIT)
    message(FATAL_ERROR "Could not find pthreads")
endif(NOT CMAKE_USE_PTHREADS_INIT)

#Static way of locating library
find_library(LIBRT_LIBRARY rt)

if(NOT LIBRT_LIBRARY)
    message(FATAL_ERROR "Librt could not be found")
endif(NOT LIBRT_LIBRARY)

#Add the libuv-dir. Compile this using another CMake-file?
find_library(LIBUV uv lib/libuv)

include_directories(lib/libuv/include)
add_executable(libuv_test libuv-test.c)
target_link_libraries(libuv_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(libuv_test ${LIBUV})
target_link_libraries(libuv_test ${LIBRT_LIBRARY})
