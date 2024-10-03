cmake_minimum_required(VERSION 3.20)

set(project_name crud) ## rename your project here

project(${project_name})


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall -Ofast")

#include_directories(${GLIB_INCLUDE_DIRS})
#link_directories(${GLIB_LIBRARY_DIRS})
#set(MODERN_CPP_KAFKA_INCLUDE_DIR src/event/modern-cpp-kafka/include)
#set(MODERN_CPP_KAFKA_PROJECT_DIR src/event/modern-cpp-kafka)

#add_subdirectory(${MODERN_CPP_KAFKA_PROJECT_DIR})
#include_directories(${MODERN_CPP_KAFKA_INCLUDE_DIR})


add_library(${project_name}-lib
        src/controller/MyController.hpp
        src/websocket/WSListener.hpp
        src/websocket/WSListener.cpp
        src/AppComponent.hpp
       )

## include directories

target_include_directories(${project_name}-lib PUBLIC src)

set(UCIADAPTER_INCLUDE_DIR uciadapter/src)
set(UCIADAPTER_PROJECT_DIR uciadapter)

add_subdirectory(${UCIADAPTER_PROJECT_DIR})
include_directories(${UCIADAPTER_INCLUDE_DIR})



## link libs
#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLIB REQUIRED glib-2.0)

find_package(oatpp              1.4.0  REQUIRED)
find_package(oatpp-websocket    1.4.0  REQUIRED)

target_link_libraries(${project_name}-lib
        # Oat++
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-websocket
     )


# If CMake can't find SQLite3:
#
# 1. Make sure that you've built oatpp-sqlite with -DOATPP_SQLITE_AMALGAMATION=ON flag
# 2. If you are not willing to use SQLite amalgamation then uncomment the following lines:
#
#find_package(SQLite3 REQUIRED)
#
#target_link_libraries(crud-lib
#        PUBLIC SQLite::SQLite3
#)

add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

        ## SQLite database file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"
        ## SQLite database test file
        -DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(${project_name}-lib INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
endif()

## add executables

add_executable(websocketd src/App.cpp)
target_link_libraries(websocketd ${project_name}-lib uciadapter)

# Testing
add_executable(${project_name}-test
        test/tests.cpp
        test/WSTest.cpp
        test/WSTest.hpp
)
target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

enable_testing()
add_test(tests ${project_name}-test)
