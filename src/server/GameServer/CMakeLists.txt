cmake_minimum_required(VERSION 2.6)
project(GAMESERVER)

#MESSAGE
message(STATUS "This is SOURCE dir " ${GAMESERVER_SOURCE_DIR})

include(${GAMESERVER_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${GAMESERVER_SOURCE_DIR})
ODIN_INSTALL_PATH(${GAMESERVER_SOURCE_DIR})
ODIN_LIBRARY_PATH(${GAMESERVER_SOURCE_DIR})

#INCLUDE
include_directories(${GAMESERVER_SOURCE_DIR}/include)

#SOURCE
aux_source_directory(${GAMESERVER_SOURCE_DIR}/. MAIN_SRC)
aux_source_directory(${GAMESERVER_SOURCE_DIR}/source SUB_SRC)

#GENERATE EXE
add_executable(gameserver ${MAIN_SRC} ${SUB_SRC})

#LINK LIBRARIES
target_link_libraries(gameserver core rt shared network lua dl z crypt
  protobuf message zlog pthread bson-1.0 mongoc-1.0 mongoc-priv)

