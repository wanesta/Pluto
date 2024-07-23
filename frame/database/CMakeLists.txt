
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_PATH}/bin)
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(HIREDIS REQUIRED hiredis)
PKG_CHECK_MODULES(LIBEVENT REQUIRED libevent)

include_directories(${PROJECT_ROOT_PATH}/utils/plog)
include_directories(${HIREDIS_INCLUDE_DIRS} ${LIBEVENT_INCLUDE_DIRS})
link_directories(${HIREDIS_LIBRARY_DIRS} ${LIBEVENT_LIBRARY_DIRS})
link_directories(${PROJECT_BINARY_PATH}/lib)

ADD_EXECUTABLE(RedisAsyncExample main.cc AsyncRedisClient.cpp AsyncMySQLClient.cpp)
target_link_libraries(RedisAsyncExample ${HIREDIS_LIBRARIES} ${LIBEVENT_LIBRARIES} pthread -lPLog mysqlclient)