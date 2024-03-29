cmake_minimum_required(VERSION 3.15)
project(feather)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++20")
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

if (MSVC)
  	add_compile_options(/bigobj)
endif ()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines -pthread -std=c++20")
    #-ftree-slp-vectorize with coroutine cause link error. disable it util gcc fix.
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-tree-slp-vectorize")
endif()

add_subdirectory(ormpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories("cinatra/include" ${OPENSSL_INCLUDE_DIR})

if (ENABLE_MYSQL)
	target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBRARY})
	target_include_directories(${PROJECT_NAME} PRIVATE ${MYSQL_INCLUDE_DIR} ormpp)
	if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MD")
	endif()
endif()

if (ENABLE_SQLITE3)
	target_link_libraries(feather sqlite3 -ldl)
	target_include_directories(feather PRIVATE ormpp)
endif()

if (ENABLE_PG)
	target_link_libraries(${PROJECT_NAME} ${PGSQL_LIBRARY})
	target_include_directories(${PROJECT_NAME} PRIVATE ${PGSQL_INCLUDE_DIR} ormpp)
endif()

if (ENABLE_SSL)
	target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES} pthread -ldl)
endif()

if (ENABLE_CLIENT_SSL)
	target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES} pthread -ldl)
endif()

if (ENABLE_GZIP)
	target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
endif()

install(TARGETS feather DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})