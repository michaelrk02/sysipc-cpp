cmake_minimum_required(VERSION 3.0)

set(project "sysipc")
set(source_files
    "file_dispatch"
    "router"
    "server"
    "client"
)

set(source_files_new "")
foreach(source_file IN LISTS source_files)
    set(source_files_new ${source_files_new} "${CMAKE_CURRENT_SOURCE_DIR}/src/${source_file}.cpp")
endforeach(source_file)

set(cpp98_flag "")
if(NOT MSVC)
    set(cpp98_flag "-std=c++98")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/distrib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/distrib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/distrib")

set(SYSIPC_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include")
set(SYSIPC_COMPILE_OPTIONS ${cpp98_flag})

project(${project})
add_library(${project} SHARED ${source_files_new})
target_include_directories(${project} PUBLIC ${SYSIPC_INCLUDE_DIRECTORIES})
target_compile_options(${project} PUBLIC ${SYSIPC_COMPILE_OPTIONS})
target_compile_definitions(${project} PUBLIC "__SYSIPC_BUILD")

add_subdirectory("test-server")
add_subdirectory("test-client")

