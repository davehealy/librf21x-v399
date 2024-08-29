option(BUILD_WRAP_csharp "Build wrap of csharp." OFF)

find_package(SWIG)

if(NOT (SWIG_FOUND))
    return()
endif()

include(${SWIG_USE_FILE})

include_directories(../)
include_directories(../../gsp)

add_definitions(-DSWIG)

set_source_files_properties(../gsp.i PROPERTIES CPLUSPLUS ON)
swig_add_module(gsp-csharp csharp ../gsp.i ../gsp-wrap.cpp)
swig_link_libraries(gsp-csharp gsp-api)

install(FILES
    gsp-test.cs
    DESTINATION gsp-wrap/gsp-csharp)
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
    DESTINATION gsp-wrap/gsp-csharp
    FILES_MATCHING REGEX ".+\\.cs$"
    PATTERN CMakeFiles EXCLUDE)
if(WIN32)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/gsp-csharp.dll
        DESTINATION gsp-wrap/gsp-csharp
        RENAME gsp.dll)
else()
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/gsp-csharp.so
        DESTINATION gsp-wrap/gsp-csharp
        RENAME gsp.so)
endif()
