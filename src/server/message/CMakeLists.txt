cmake_minimum_required(VERSION 2.6)
project(MESSAGE)

############################
####for proto buffer begin
############################
SET(PROTO_DIR ${MESSAGE_SOURCE_DIR}/proto)
FILE(GLOB PROTO_INPUT ${PROTO_DIR}/*.proto)
FIND_PROGRAM(PROTOC protoc PATHS ${MESSAGE_SOURCE_DIR}/)
SET(PROTOC_C_OUT_FLAG --cpp_out)
set(PROTO_GEN_DIR ${MESSAGE_SOURCE_DIR}/source)

#message(status ${PROTO_GEN_DIR})

FOREACH(PROTO_FILE ${PROTO_INPUT})
GET_FILENAME_COMPONENT(PROTO_NAME ${PROTO_FILE} NAME_WE)
SET(CUR_PROTO_GEN
${PROTO_GEN_DIR}/${PROTO_NAME}.pb.h
${PROTO_GEN_DIR}/${PROTO_NAME}.pb.cc
)
SET(PROTO_GEN
${PROTO_GEN}
${CUR_PROTO_GEN}
)
ADD_CUSTOM_COMMAND(
OUTPUT ${CUR_PROTO_GEN}
COMMAND ${PROTOC} ${PROTO_FILE} ${PROTOC_C_OUT_FLAG} ${PROTO_GEN_DIR} -I${PROTO_DIR}
DEPENDS  ${PROTO_FILE}
WORKING_DIRECTORY ${MESSAGE_SOURCE_DIR}/
)
endforeach(PROTO_FILE ${PROTO_INPUT})

SET_SOURCE_FILES_PROPERTIES(
${PROTO_GEN}
PROPERTIES
GENERATED TRUE)
#SET OUT PATH
#SET(LIBRARY_OUTPUT_PATH ${MESSAGE_SOURCE_DIR}/lib)
############################
####for proto buffer end 
############################

#MESSAGE
message(STATUS "This is SOURCE dir " ${MESSAGE_SOURCE_DIR})

include(${MESSAGE_SOURCE_DIR}/../../server.cmake)

ODIN_BUILD_OPTION()
ODIN_BUILD()
ODIN_COMMON_INCLUDE(${MESSAGE_SOURCE_DIR})
ODIN_INSTALL_PATH(${MESSAGE_SOURCE_DIR})
ODIN_LIBRARY_PATH(${MESSAGE_SOURCE_DIR})

#SOURCE
aux_source_directory(${MESSAGE_SOURCE_DIR}/. MESSAGE_SRC)

#ADD -l
add_definitions(-lrt)

#ADD LIBRARY
add_library(message STATIC ${PROTO_GEN} ${MESSAGE_SRC})

#LINK LIBRARIES
#target_link_libraries(message protobuf)
