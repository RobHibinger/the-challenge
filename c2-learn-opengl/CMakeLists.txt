cmake_minimum_required(VERSION 3.31.0)
project(learn-opengl VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(learn-opengl main.cpp)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(learn-opengl PRIVATE SDL3::SDL3)

find_package(glad CONFIG REQUIRED)
target_link_libraries(learn-opengl PRIVATE glad::glad)