cmake_minimum_required(VERSION 3.0)
project(feather)

#for mac
#SET(CMAKE_C_COMPILER "/usr/local/bin/gcc-8")
#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-8")
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")
SET(ENABLE_MYSQL ON)
SET(ENABLE_PG OFF)
SET(ENABLE_SQLITE3 OFF)

SET(ENABLE_GZIP OFF)

SET(ENABLE_SSL OFF)

SET(ENABLE_CLIENT_SSL OFF)

if (ENABLE_MYSQL)
	add_definitions(-DORMPP_ENABLE_MYSQL)
endif()

if (ENABLE_SQLITE3)
	add_definitions(-DORMPP_ENABLE_SQLITE3)
endif()

if (ENABLE_PG)
	add_definitions(-DORMPP_ENABLE_PG)
endif()

if (ENABLE_SSL)
	add_definitions(-DCINATRA_ENABLE_SSL)
	message(STATUS "Use SSL")
endif()

if(ENABLE_GZIP)
	add_definitions(-DCINATRA_ENABLE_GZIP)
endif()

if(ENABLE_CLIENT_SSL)
	add_definitions(-DCINATRA_ENABLE_CLIENT_SSL)
endif()

find_package(Boost 1.60 REQUIRED COMPONENTS system filesystem)

if (ENABLE_SSL)
	find_package(OpenSSL REQUIRED)
endif()

if (ENABLE_CLIENT_SSL)
	find_package(OpenSSL REQUIRED)
endif()

if (ENABLE_GZIP)
	find_package(ZLIB REQUIRED)
endif()


set(SOURCE_FILES
        main.cpp
)

add_subdirectory(ormpp)

add_executable(feather ${SOURCE_FILES})

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

target_link_libraries(feather ${Boost_LIBRARIES} uuid -lstdc++fs)

if (ENABLE_MYSQL)
	target_link_libraries(feather ${MYSQL_LIBRARY})
	target_include_directories(feather PRIVATE ${MYSQL_INCLUDE_DIR} ormpp)
endif()

if (ENABLE_SQLITE3)
	target_link_libraries(feather sqlite3)
	target_include_directories(feather PRIVATE ormpp)
endif()

if (ENABLE_PG)
	target_link_libraries(feather ${PGSQL_LIBRARY})
	target_include_directories(feather PRIVATE ${PGSQL_INCLUDE_DIR} ormpp)
endif()

if (ENABLE_SSL)
	target_link_libraries(feather ${OPENSSL_LIBRARIES} pthread -ldl)
endif()

if (ENABLE_CLIENT_SSL)
	target_link_libraries(feather ${OPENSSL_LIBRARIES} pthread -ldl)
endif()

if (ENABLE_GZIP)
	target_link_libraries(feather ${ZLIB_LIBRARIES})
endif()

install(TARGETS feather DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})