cmake_minimum_required(VERSION 2.6)
project(BOTS)

#MESSAGE
message(STATUS "This is SOURCE dir " ${BOTS_SOURCE_DIR})

include(${BOTS_SOURCE_DIR}/../../odin.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${BOTS_SOURCE_DIR}/../)
#ODIN_INSTALL_PATH(${BOTS_SOURCE_DIR}/../)
ODIN_LIBRARY_PATH(${BOTS_SOURCE_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${BOTS_SOURCE_DIR}/bin)

#INCLUDE
include_directories(${BOTS_SOURCE_DIR}/src)

#SOURCE
aux_source_directory(${BOTS_SOURCE_DIR}/src MAIN_SRC)

#GENERATE EXE
add_executable(bots ${MAIN_SRC} )

#LINK LIBRARIES
target_link_libraries(bots rt shared lua dl z crypt
  protobuf message curl)

