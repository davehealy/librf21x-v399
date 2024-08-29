cmake_minimum_required(VERSION 2.8)

add_subdirectory(hidapi)
find_package(Threads REQUIRED)

include_directories(hidapi ../tthread)
set(gsp_SRC gsp.cpp abstract-gsp.cpp gsp-hid.cpp ../tthread/tinythread.cpp)
set(gsp_LIB hidapi)

if(WIN32)
    include_directories(../SimpleSocket)
    set(gsp_SRC ${gsp_SRC}
        gsp-serial-windows.cpp Serial/Serial.cpp Serial/SerialEx.cpp
        gsp-socket.cpp ../SimpleSocket/ActiveSocket.cpp ../SimpleSocket/SimpleSocket.cpp
    )
    set(gsp_LIB ${gsp_LIB}
        ws2_32.lib winmm.lib
    )
elseif(APPLE)
    set(gsp_SRC ${gsp_SRC}
    )
elseif(UNIX)
    include_directories(../SimpleSocket)
    add_definitions(-D_LINUX -D_GNU_SOURCE)
    set(gsp_SRC ${gsp_SRC}
        gsp-serial-unix.cpp cssl/cssl.c
        gsp-socket.cpp ../SimpleSocket/ActiveSocket.cpp ../SimpleSocket/SimpleSocket.cpp
    )
endif()

add_library(gsp STATIC ${gsp_SRC})
target_link_libraries(gsp ${CMAKE_THREAD_LIBS_INIT} ${gsp_LIB})
