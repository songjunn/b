cmake_minimum_required(VERSION 2.6)
project(DATASERVER)

#MESSAGE
message(STATUS "This is SOURCE dir " ${DATASERVER_SOURCE_DIR})

include(${DATASERVER_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${DATASERVER_SOURCE_DIR})
ODIN_INSTALL_PATH(${DATASERVER_SOURCE_DIR})
ODIN_LIBRARY_PATH(${DATASERVER_SOURCE_DIR})

#INCLUDE
include_directories(${DATASERVER_SOURCE_DIR}/include)
include_directories(${DATASERVER_SOURCE_DIR}/../common/template)

#SOURCE
aux_source_directory(${DATASERVER_SOURCE_DIR}/. MAIN_SRC)
aux_source_directory(${DATASERVER_SOURCE_DIR}/source SUB_SRC)

#GENERATE EXE
add_executable(dataserver ${MAIN_SRC} ${SUB_SRC})

#LINK LIBRARIES
target_link_libraries(dataserver core rt shared network lua dl z crypt
  protobuf message zlog pthread bson-1.0 mongoc-1.0 mongoc-priv)
