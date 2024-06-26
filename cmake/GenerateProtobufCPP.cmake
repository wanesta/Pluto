#
# Locate and configure the Google Protocol Buffers library
#
# Adds the following targets:
#
#  protobuf::libprotobuf - Protobuf library
#  protobuf::libprotobuf-lite - Protobuf lite library
#  protobuf::libprotoc - Protobuf Protoc Library
#  protobuf::protoc - protoc executable
#

#
# Generates C++ sources from the .proto files
#
# protobuf_generate_cpp (<SRCS> <HDRS> <DEST> [<ARGN>...])
#
#  SRCS - variable to define with autogenerated source files
#  HDRS - variable to define with autogenerated header files
#  DEST - directory where the source files will be created
#  ARGN - .proto files
#
function(GENERATE_PROTOBUF_CPP SRCS HDRS DEST)
    if(NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_CPP() called without any proto files")
        return()
    endif()

    if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
        # Create an include path for each file specified
        foreach(FIL ${ARGN})
            get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
            get_filename_component(ABS_PATH ${ABS_FIL} PATH)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    else()
        set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if(DEFINED PROTOBUF_IMPORT_DIRS)
        foreach(DIR ${PROTOBUF_IMPORT_DIRS})
            get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if(${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif()
        endforeach()
    endif()

    set(${SRCS})
    set(${HDRS})
    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        get_filename_component(FIL_PATH ${FIL} PATH)

        list(APPEND ${SRCS} "${DEST}/${FIL_WE}.pb.cc")
        list(APPEND ${HDRS} "${DEST}/${FIL_WE}.pb.h")

        add_custom_command(
                OUTPUT "${DEST}/${FIL_WE}.pb.cc"
                "${DEST}/${FIL_WE}.pb.h"
                COMMAND protobuf::protoc
                ARGS --cpp_out ${DEST}
                -I ${FIL_PATH}
                ${ABS_FIL}
                DEPENDS ${ABS_FIL})
    endforeach()

    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()


function(DELETE_OLD_GRPC_PB PATH_FILES)
    foreach(file ${PATH_FILES})
        # 判断文件是否存在
        if(EXISTS "${file}")
            # 如果存在则删除文件
            message("Deleting file: ${file}")
            file(REMOVE "${file}")
        else()
            # 如果不存在则打印消息
            message("File not found: ${file}")
        endif()
    endforeach()
endfunction()