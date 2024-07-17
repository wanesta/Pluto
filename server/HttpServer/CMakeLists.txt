set(server_start server_start.cc)

set(lightgbm_so /usr/local/lib/lib_lightgbm.so)
find_package(OpenSSL REQUIRED)
find_package(wfrest REQUIRED CONFIG HINTS ..)
find_package(workflow REQUIRED CONFIG HINTS ..)
link_directories(${WFREST_LIB_DIR} ${WORKFLOW_LIB_DIR})
#find_package(Boost REQUIRED COMPONENTS system thread)

if (APPLE)
    set(WFREST_LIB wfrest workflow pthread OpenSSL::SSL OpenSSL::Crypto protobuf z)
else ()
    set(WFREST_LIB wfrest)
endif ()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_PATH}/bin)

include_directories(${PROJECT_ROOT_PATH}/utils/plog)
include_directories(${PROJECT_ROOT_PATH}/model/LightGBM)
link_directories(${PROJECT_BINARY_PATH}/lib)

ADD_EXECUTABLE(server_start ${server_start})
target_link_libraries(server_start ${WFREST_LIB} -lPLog -lMODEL_GBM)