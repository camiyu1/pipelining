cmake_minimum_required(VERSION 3.5)
project(pipeline)
set (CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)

if (CMAKE_USE_PTHREADS_INIT)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g3 -ggdb3")
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
endif()

set(CMAKE_CXX_STANDARD 17)

# Library for SapeonRuntime
set(TARGET_SERVER pipeline)
add_executable(${TARGET_SERVER}
  main.cpp
)

target_link_libraries(${TARGET_SERVER} PRIVATE glog benchmark::benchmark)

