cmake_minimum_required(VERSION 3.10)
project(Raytrace)
set(CMAKE_CXX_STANDARD 17)
add_executable(Raytrace main.cpp)
target_link_libraries(Raytrace pthread)

