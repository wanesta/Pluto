set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_PATH}/bin/model/alphaFM)
set(UTILS ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils/utils.cpp)
set(PC_FRAME ${CMAKE_CURRENT_SOURCE_DIR}/src/Frame/pc_frame.cpp)

#ADD_SUBDIRECTORY(src/Frame)
find_package(Threads REQUIRED)

add_executable(fm_train fm_train.cpp ${UTILS} ${PC_FRAME})
target_link_libraries(fm_train PRIVATE pthread)

add_executable(fm_predict fm_predict.cpp ${UTILS} ${PC_FRAME})
target_link_libraries(fm_predict PRIVATE pthread)
