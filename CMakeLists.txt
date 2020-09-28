# 2020-07-25

cmake_minimum_required(VERSION 3.12.4)
project(v2vk C CXX)
set(CMAKE_CXX_STANDARD 17)


if(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    execute_process(COMMAND export OPENSSL_ROOT_DIR=/usr/local/opt/openssl WORKING_DIRECTORY ./)
endif()

find_library(LIBRT rt)
find_package(OpenSSL REQUIRED)
find_package(workflow REQUIRED CONFIG HINTS ..)
include_directories(${OPENSSL_INCLUDE_DIR} ${WORKFLOW_INCLUDE_DIR})
link_directories(${WORKFLOW_LIB_DIR})

if (WIN32)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /MP /wd4200")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4200 /std:c++14")
else ()
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++11 -fno-exceptions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=c++17 -fno-exceptions")
endif ()

if (APPLE)
    set(WORKFLOW_LIB workflow pthread OpenSSL::SSL OpenSSL::Crypto)
else ()
    set(WORKFLOW_LIB workflow ${LIBRT})
endif ()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/include)


#set(HTTPLIB_DEMO demo/httplib_demo.cpp)
#add_executable(httplib_demo ${HTTPLIB_DEMO})
set(ANNOY_DEMO demo/annoy_demo.cpp)
add_executable(annoy_demo ${ANNOY_DEMO})
set(ANNOY_V2V_SERVER src/v2v_server/annoy_v2v_server.cpp)
add_executable(annoy_v2v_server ${ANNOY_V2V_SERVER})
target_link_libraries(annoy_v2v_server ${WORKFLOW_LIB})

