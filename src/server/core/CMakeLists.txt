cmake_minimum_required(VERSION 2.6)
project(CORE)

#MESSAGE
message(STATUS "This is SOURCE dir " ${CORE_SOURCE_DIR})

include(${CORE_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${CORE_SOURCE_DIR})
ODIN_INSTALL_PATH(${CORE_SOURCE_DIR})
ODIN_LIBRARY_PATH(${CORE_SOURCE_DIR})

#SOURCE
aux_source_directory(${CORE_SOURCE_DIR}/source CORE_SRC)

#LIBRARY
add_library(core STATIC ${CORE_SRC})
