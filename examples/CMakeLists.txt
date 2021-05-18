cmake_minimum_required(VERSION 3.17)
project(examples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Weffc++")

include_directories(../include)

add_subdirectory(../ libfd)

link_libraries(libfd)

add_executable(
        AutoClosingFileDescriptor
        src/AutoClosingFileDescriptor.cpp
)

add_executable(
        CommunicationBetweenThreads
        src/CommunicationBetweenThreads.cpp
)

target_link_libraries(
        CommunicationBetweenThreads
            PRIVATE
            pthread
)

add_executable(
        DataOnEvent
        src/DataOnEvent.cpp
)

add_executable(
        LoopPlusTrackedStdin
        src/LoopPlusTrackedStdin.cpp
)

add_executable(
    TrackStdin
        src/TrackStdin.cpp
)

add_executable(
        UsingPipe
        src/UsingPipe.cpp
)

