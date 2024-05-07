include(${PROJECT_ROOT_PATH}/cmake/GenerateProtobufCPP.cmake)
include(${PROJECT_ROOT_PATH}/cmake/GenerateGRPCCPP.cmake)
include(${PROJECT_ROOT_PATH}/cmake/common.cmake)
#include(${PROJECT_ROOT_PATH}/cmake/FindGRPC.cmake)
#include(${PROJECT_ROOT_PATH}/cmake/FindProtobuf.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_PATH}/bin)

set(protofiles
        ${PROJECT_ROOT_PATH}/proto/helloworld.proto
        ${PROJECT_ROOT_PATH}/proto/model.proto
        ${PROJECT_ROOT_PATH}/proto/predict.proto
        ${PROJECT_ROOT_PATH}/proto/resource_handle.proto
        ${PROJECT_ROOT_PATH}/proto/tensor.proto
        ${PROJECT_ROOT_PATH}/proto/tensor_shape.proto
        ${PROJECT_ROOT_PATH}/proto/types.proto
    )
set(PROTO_SRC_DIR ${PROJECT_ROOT_PATH}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})
include_directories(${PROJECT_ROOT_PATH}/utils/plog)

file(GLOB PB_H_FILES "${PROTO_SRC_DIR}/*.pb.h")
file(GLOB PB_CC_FILES "${PROTO_SRC_DIR}/*.pb.cc")

DELETE_OLD_GRPC_PB("${PB_H_FILES}")
DELETE_OLD_GRPC_PB("${PB_CC_FILES}")

GENERATE_PROTOBUF_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${protofiles})
GENERATE_GRPC_CPP(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${protofiles})
#PROTOBUF_GENERATE_CPP(PRO_SRC PRO_HDS EXPORT_MACRO DLL_EXPORT ${protofiles})
link_directories(${PROJECT_BINARY_PATH}/lib)

add_library(hello_world
            ${PROTO_SRCS}
            ${PROTO_HDRS}
            ${PRO_SRC}
            ${PRO_HDS}
            ${GRPC_SRCS}
            ${GRPC_HDRS})

target_link_libraries(hello_world
        ${_GRPC_GRPCPP_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        ${PROTOBUF_LIBRARIES})

foreach(_target
        greeter_client greeter_server
        greeter_async_client greeter_async_client2
        greeter_async_server)
    add_executable(${_target} "${_target}.cc")
    target_link_libraries(${_target}
            -lPLog
            hello_world
            ${_REFLECTION}
            ${_GRPC_GRPCPP}
            ${_PROTOBUF_LIBPROTOBUF})
endforeach()