cmake_minimum_required(VERSION 3.0)
project(feather)

#for mac
#SET(CMAKE_C_COMPILER "/usr/local/bin/gcc-8")
#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-8")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")
SET(ENABLE_MYSQL ON)
SET(ENABLE_PG OFF)
SET(ENABLE_SQLITE3 OFF)

if (ENABLE_MYSQL)
add_definitions(-DORMPP_ENABLE_MYSQL)
endif()

if (ENABLE_SQLITE3)
add_definitions(-DORMPP_ENABLE_SQLITE3)
endif()

if (ENABLE_PG)
add_definitions(-DORMPP_ENABLE_PG)
endif()

find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)

set(SOURCE_FILES
        main.cpp
)

add_executable(feather ${SOURCE_FILES})

if (ENABLE_MYSQL)
target_link_libraries(feather mysqlclient -lstdc++fs ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_LIB})
endif()

if (ENABLE_SQLITE3)
target_link_libraries(feather sqlite3 -lstdc++fs ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_LIB})
endif()

if (ENABLE_PG)
target_link_libraries(feather pq -lstdc++fs ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_LIB})
endif()
