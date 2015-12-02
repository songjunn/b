cmake_minimum_required(VERSION 2.6)
project(BISERVER)

#MESSAGE
message(STATUS "This is SOURCE dir " ${BISERVER_SOURCE_DIR})

include(${BISERVER_SOURCE_DIR}/../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${BISERVER_SOURCE_DIR})
ODIN_INSTALL_PATH(${BISERVER_SOURCE_DIR})
ODIN_LIBRARY_PATH(${BISERVER_SOURCE_DIR})

#INCLUDE
include_directories(${BISERVER_SOURCE_DIR}/include)

#SOURCE
aux_source_directory(${BISERVER_SOURCE_DIR}/. MAIN_SRC)
aux_source_directory(${BISERVER_SOURCE_DIR}/source SUB_SRC)

#GENERATE EXE
add_executable(biserver ${MAIN_SRC} ${SUB_SRC})

#LINK LIBRARIES
target_link_libraries(biserver servercore rt shared lua dl z crypt
  protobuf message mongoclient boost_system boost_filesystem boost_thread)

