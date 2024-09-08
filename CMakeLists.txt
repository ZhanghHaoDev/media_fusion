cmake_minimum_required(VERSION 3.30)
project(media_fusion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(bin)
add_subdirectory(test)
add_subdirectory(qt_interface)
add_subdirectory(file_management)
add_subdirectory(media_transcoding)
