cmake_minimum_required(VERSION 2.6)
project(GATESERVER)

#MESSAGE
message(STATUS "This is SOURCE dir " ${GATESERVER_SOURCE_DIR})

include(${GATESERVER_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${GATESERVER_SOURCE_DIR})
ODIN_INSTALL_PATH(${GATESERVER_SOURCE_DIR})
ODIN_LIBRARY_PATH(${GATESERVER_SOURCE_DIR})

#INCLUDE
include_directories(${GATESERVER_SOURCE_DIR}/include)

#SOURCE
aux_source_directory(${GATESERVER_SOURCE_DIR}/. MAIN_SRC)
aux_source_directory(${GATESERVER_SOURCE_DIR}/source SUB_SRC)

#GENERATE EXE
add_executable(gateserver ${MAIN_SRC} ${SUB_SRC})

#LINK LIBRARIES
target_link_libraries(gateserver core rt curl shared network lua dl z protobuf 
	message zlog pthread bson-1.0 mongoc-1.0 mongoc-priv)

