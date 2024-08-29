cmake_minimum_required(VERSION 2.8)

option(BUILD_GSP_WRAP "Build C and C++ wrap of gsp for low level communication usage." OFF)
if(NOT BUILD_GSP_WRAP)
    return()
endif()

if(MSVC)
    add_definitions(-DLIBGSP_EXPORTS)
endif()

add_library(gsp-api SHARED gsp-api.cpp gsp-api.def)
target_link_libraries(gsp-api gsp)
include_directories(../tthread ../gsp)

install(TARGETS gsp-api DESTINATION gsp-api)
install(FILES gsp-api.h DESTINATION gsp-api)


add_executable(gsp-wrap-test gsp-wrap.cpp gsp-wrap-test.cpp)
target_link_libraries(gsp-wrap-test gsp-api)

install(TARGETS gsp-wrap-test DESTINATION gsp-api-wrap-cpp)
install(FILES gsp-wrap.h gsp-wrap.cpp DESTINATION gsp-api-wrap-cpp)

#add_subdirectory(gsp-csharp)
