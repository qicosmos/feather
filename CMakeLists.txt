cmake_minimum_required(VERSION 3.0)
project(feather)

#for mac
#SET(CMAKE_C_COMPILER "/usr/local/bin/gcc-8")
#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-8")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")
SET(ENABLE_MYSQL ON)
SET(ENABLE_PG OFF)
SET(ENABLE_SQLITE3 OFF)
find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)

set(SOURCE_FILES
        main.cpp
)

add_executable(feather ${SOURCE_FILES})
target_link_libraries(feather mysqlclient sqlite3 pq -lstdc++fs ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_LIB})
