# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.5)

set(ENV{CMAKE_EXPORT_COMPILE_COMMANDS} 1)
# 项目信息
project(cqlite)

aux_source_directory(./src DIR_SRCS)

add_executable(cqlite ${DIR_SRCS})
