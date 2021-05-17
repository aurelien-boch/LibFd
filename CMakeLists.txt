cmake_minimum_required(VERSION 3.17)
project(libfd)

set(CMAKE_CXX_STANDARD 20)

add_library(
    libfd
        src/FdMonitor.cpp
        src/TimerFd.cpp
        src/StateFd.cpp
        src/Pipe.cpp
)

target_include_directories(
    libfd
        PRIVATE
        include
)

set_target_properties(
    libfd
        PROPERTIES
        PREFIX          ""
        CMAKE_CXX_FLAGS "-Wall -Wextra -Weffc++"
)
