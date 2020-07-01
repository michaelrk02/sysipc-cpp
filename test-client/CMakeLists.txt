cmake_minimum_required(VERSION 3.0)

set(project "test-client")

project(${project})
add_executable(${project} "main.cpp")
target_include_directories(${project} PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${project} "sysipc")

