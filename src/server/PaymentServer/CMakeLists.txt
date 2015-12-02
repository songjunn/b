cmake_minimum_required(VERSION 2.6)
project(PAYMENTSERVER)

#MESSAGE
message(STATUS "This is SOURCE dir " ${PAYMENTSERVER_SOURCE_DIR})

include(${PAYMENTSERVER_SOURCE_DIR}/../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${PAYMENTSERVER_SOURCE_DIR})
ODIN_INSTALL_PATH(${PAYMENTSERVER_SOURCE_DIR})
ODIN_LIBRARY_PATH(${PAYMENTSERVER_SOURCE_DIR})

#INCLUDE
include_directories(${PAYMENTSERVER_SOURCE_DIR}/include)

#SOURCE
aux_source_directory(${PAYMENTSERVER_SOURCE_DIR}/. MAIN_SRC)
aux_source_directory(${PAYMENTSERVER_SOURCE_DIR}/source SUB_SRC)

#GENERATE EXE
add_executable(paymentserver ${MAIN_SRC} ${SUB_SRC})

#LINK LIBRARIES
target_link_libraries(paymentserver servercore rt shared lua dl z crypt
  protobuf common mongoclient boost_system boost_filesystem boost_thread
  mysqlclient curl jsoncpp)

