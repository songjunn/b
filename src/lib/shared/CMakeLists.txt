cmake_minimum_required(VERSION 2.6)
project(SHARED)

#MESSAGE
message(STATUS "This is SOURCE dir " ${SHARED_SOURCE_DIR})

include(${SHARED_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${SHARED_SOURCE_DIR})
ODIN_INSTALL_PATH(${SHARED_SOURCE_DIR})
ODIN_LIBRARY_PATH(${SHARED_SOURCE_DIR})

#SOURCE
aux_source_directory(${SHARED_SOURCE_DIR}/config SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/container SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/nedmalloc SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/rapidjson SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/thread SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/util SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/xml SHARED_SRC)
aux_source_directory(${SHARED_SOURCE_DIR}/httpd SHARED_SRC)

#ADD -l
add_definitions(-lrt)

#ADD LIBRARY
add_library(shared STATIC ${SHARED_SRC})
