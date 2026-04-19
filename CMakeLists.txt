cmake_minimum_required(VERSION 3.10)
project(magb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB magb_src "src/*.cpp" "src/core/*.cpp" "src/frontend/raylib/*.cpp")

add_executable(magb ${magb_src})

target_include_directories(magb PRIVATE src)
target_include_directories(magb PRIVATE thirdparty)

add_subdirectory(thirdparty/raylib)

target_link_libraries(magb raylib)

if (NOT WIN32)
    target_compile_options(magb PRIVATE -Wall -Wextra -pedantic)
endif()
