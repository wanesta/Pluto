set(YMLSRC YamlParse.cc)
set(YMLHDR YamlParse.h)


find_package(yaml-cpp REQUIRED)

include_directories(/usr/local/include/yaml-cpp)

link_directories(${PROJECT_BINARY_PATH}/lib)
ADD_LIBRARY(yamlpar_SHARED STATIC ${YMLSRC} ${YMLHDR})

target_link_libraries(yamlpar_SHARED PRIVATE ULOG_STATIC /usr/local/lib/libyaml-cpp.a)


SET_TARGET_PROPERTIES(yamlpar_SHARED PROPERTIES OUTPUT_NAME "yamlpar")
SET_TARGET_PROPERTIES(yamlpar_SHARED PROPERTIES CLEAN_DERECT_OUTPUT 1)
set_target_properties(yamlpar_SHARED PROPERTIES PUBLIC_HEADER ${PROJECT_BINARY_DIR}/include/YamlParse.h)