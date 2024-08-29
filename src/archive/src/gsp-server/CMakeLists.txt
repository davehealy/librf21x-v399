cmake_minimum_required(VERSION 2.8)

option(BUILD_GSP_SERVER "Build GSP server listener for communication between socket and serial port." OFF)
if(NOT BUILD_GSP_SERVER)
    return()
endif()

include_directories(../gsp ../tthread)
if(WIN32)
    add_executable(gsp-server gsp-socket-server-main.cpp
        ../SimpleSocket/ActiveSocket.cpp ../SimpleSocket/PassiveSocket.cpp ../SimpleSocket/SimpleSocket.cpp)
    include_directories(../SimpleSocket)
else()
    add_executable(gsp-server gsp-socket-server-main-linux.cpp)
endif()
target_link_libraries(gsp-server gsp)

install(TARGETS gsp-server DESTINATION .)
