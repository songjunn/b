cmake_minimum_required(VERSION 2.6)

include_directories(include)
include_directories(.)
#include_directories(../external/mysql/include/)
#include_directories(../external/lua/include/)
#add_subdirectory(source)
aux_source_directory(source LUA_SRC)

add_library(lua ${LUA_SRC})
