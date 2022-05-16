cmake_minimum_required(VERSION 3.19)

project(Fork VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(forkLabs forkLabs.cpp)

add_executable(forkPipe ForkPipe.cpp)
target_link_libraries(forkPipe forkLabs)

add_executable(forkFifo forkFifo.cpp)
target_link_libraries(forkFifo forkLabs)

#add_executable(forkSockets forkSockets.cpp)
#target_link_libraries(forkSockets forkLabs)
