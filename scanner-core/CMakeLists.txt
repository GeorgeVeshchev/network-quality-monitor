cmake_minimum_required(VERSION 3.14)
project(scanner_core CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

include_directories(include)

add_executable(network_scanner 
    src/main.cpp 
    src/thread_pool.cpp 
    src/port_scanner.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(network_scanner PRIVATE nlohmann_json::nlohmann_json Threads::Threads)