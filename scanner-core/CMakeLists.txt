cmake_minimum_required(VERSION 3.14)
project(scanner_core CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(network_scanner 
    src/main.cpp 
    src/thread_pool.cpp 
    src/scanner.cpp
)