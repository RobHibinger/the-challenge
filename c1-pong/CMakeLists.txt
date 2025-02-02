cmake_minimum_required(VERSION 3.31.0)
project(pong VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(pong main.cpp)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(pong PRIVATE SDL3::SDL3)