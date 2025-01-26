cmake_minimum_required(VERSION 3.31.0)
project(starter-template VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(starter-template main.cpp)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(starter-template PRIVATE SDL3::SDL3)