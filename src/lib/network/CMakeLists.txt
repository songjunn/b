cmake_minimum_required(VERSION 2.6)
project(NETWORK)

#MESSAGE
message(STATUS "This is SOURCE dir " ${NETWORK_SOURCE_DIR})

include(${NETWORK_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${NETWORK_SOURCE_DIR})
ODIN_INSTALL_PATH(${NETWORK_SOURCE_DIR})
ODIN_LIBRARY_PATH(${NETWORK_SOURCE_DIR})

#SOURCE
aux_source_directory(${NETWORK_SOURCE_DIR}/. NETWORK_SRC)

#ADD -l
add_definitions(-lrt)

#ADD LIBRARY
add_library(network STATIC ${NETWORK_SRC})
