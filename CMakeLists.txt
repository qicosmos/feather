cmake_minimum_required(VERSION 3.0)
project(feather)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")

find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)

set(SOURCE_FILES
        main.cpp
)

add_executable(feather ${SOURCE_FILES})
target_link_libraries(feather mysqlclient sqlite3 pq ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_LIB})
