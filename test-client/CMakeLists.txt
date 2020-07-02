cmake_minimum_required(VERSION 3.0)

set(project "test-client")

project(${project})
add_executable(${project} "main.cpp")
target_include_directories(${project} PUBLIC ${SYSIPC_INCLUDE_DIRECTORIES})
target_compile_options(${project} PUBLIC ${SYSIPC_COMPILE_OPTIONS})
target_link_libraries(${project} "sysipc")

